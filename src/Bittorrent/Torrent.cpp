//
// Created by ctuh on 9/23/23.
//
#include "Bittorrent/Torrent.hpp"
#include "Bencode/BencodeFile.hpp"
#include "Inet/RequestBuilder.hpp"
#include "Inet/RequestSender.hpp"
#include "Utils/UrlMethods.hpp"
#include "Bittorrent/TrackerResponse.hpp"
#include "Bittorrent/BittorrentMessages/HandshakeMessageBuilder.hpp"
#include "Bittorrent/BittorrentMessages/Interested.hpp"
#include "Bittorrent/BittorrentMessages/BittorrentMessageParser.hpp"
#include <iostream>
#include "Bittorrent/BittorrentMessages/Request.hpp"
#include "Bittorrent/BittorrentMessages/BitTorrentMessageTypeConverter.hpp"
#include "Bittorrent/BittorrentMessages/BitfieldReader.hpp"

Torrent::Torrent(const std::string &filepath): m_torrentFilePath(filepath),
                                               m_isFileInited(false),
                                               m_poller(std::make_unique<SocketPoller>(10,10000)) {
    m_poller->setReceiveMessageCallback([this](std::vector<std::byte> const& rawMessage, StreamSocketPtr streamSocket) {
        this->handle(rawMessage, streamSocket);
    });
}

void Torrent::addFilePieceToQueue(TorrentFile const& file) {
    Task task(file.getNextBlock(), 0);
    m_taskQueue.push(task); //TODO need to be piece
}

Torrent::~Torrent() {
    if(m_producerThread && m_producerThread->joinable()) {
        m_producerThread->join();
    }
}

void Torrent::produceTasks() {
    bool hasIncompleteFile = false;

    for(auto const& file: m_files) {
        if(file.getStatus() != TorrentFileStatus::Complete) {
            addFilePieceToQueue(file);
            if(not hasIncompleteFile)
                hasIncompleteFile = true;
        }
    }

    if(!hasIncompleteFile) {
        Logger::logInfo(std::format("Torrent downloaded"));
        m_isRunning.clear();
    }
}

void Torrent::run() {
    m_isRunning.test_and_set();
    if(!initFiles())
        return;

    while(m_isRunning.test()) {
        initPeers();
        std::thread producerThread;
        std::thread consumerThread;
        for(auto& file: m_files) {
            if(file.getStatus() == TorrentFileStatus::Complete)
                continue;
            Task task{.block = file.getNextBlock(), .begin = 0};
            m_taskQueue.push(std::move(task));
        }


    }

    m_producerThread = std::make_unique<std::thread>([this]() {
       while(m_isRunning.test()) {
           produceTasks();
       }
    });

}

void Torrent::stop() {
    m_isRunning.clear();
}

TrackerResponse Torrent::sendTrackerRequest() const {
    BencodeFile bencodeFile(m_torrentFilePath);
    std::string announce = bencodeFile.getAnnounce();
    auto request = RequestBuilder::buildTrackerRequest(bencodeFile, UrlMethods::getDomainName(announce));
    TrackerResponse response = RequestSender::sendRequest<TrackerResponse>(request, UrlMethods::getDomainName(announce), "80");
    return response;
}

std::string Torrent::getInfoHash() const {
    BencodeFile bencodeFile(m_torrentFilePath);
    auto torrentHash = bencodeFile.getInfoHash();
    std::string infoHash;
    for (auto hashEl: torrentHash) {
        infoHash.push_back(static_cast<char>(hashEl));
    }
    return infoHash;
}


std::string Torrent::createHandshakeMessage() const {
    auto infoHash = getInfoHash();
    HandshakeMessageBuilder builder;
    builder.setInfoHash(infoHash);
    return builder.getMessage();
}

void Torrent::addPeerIfConfirmHandshake(std::vector<Peer> const& peers, std::string const& handshakeMessage) {
    for (auto &peer: peers) {
        if(peer.handshake(handshakeMessage)) {
            m_poller->add(peer.getSocket());
            m_hostToPeer.emplace(peer.getAddress(), peer);
        }
    }
}

void Torrent::setTimerTimeout(std::uint32_t timeout) {
    if(!m_timer) {
        m_timer = std::make_unique<Timer>(std::chrono::seconds(timeout));
    }
    else {
        m_timer->changeTimeout(std::chrono::seconds(timeout));
    }
}

bool Torrent::initPeers() {
    if(m_timer && not m_timer->isExpired())
        return true;
    try {
        auto trackerResponse = sendTrackerRequest();
        auto handshakeMessage = createHandshakeMessage();
        addPeerIfConfirmHandshake(trackerResponse.getPeers(), handshakeMessage);
        setTimerTimeout(trackerResponse.getInterval());
    }
    catch(std::exception& e) {
        Logger::logError(std::format("Error in Torrent::initPeers() for file: {} Error: {}", m_torrentFilePath, e.what()));
        return false;
    }
    return true;
}

bool Torrent::initFiles() {
    if(m_isFileInited)
        return true;
    try {
        BencodeFile bencodeFile(m_torrentFilePath);

        auto files = bencodeFile.getFiles();

        std::size_t offset = 0;
        for (auto file: files) {
            BencodeParser fileParser(file);
            std::size_t fileSize = std::stoi(fileParser["length"]);
            std::string filePath = fileParser["path"];
            m_files.emplace_back(filePath, fileSize, offset);
            offset += fileSize;
        }

        m_pieceLength = std::stoi(bencodeFile.getPieceLength());
    }
    catch(std::exception& e) {
        Logger::logError(std::format("Error in Torrent::initFiles() for file: {} Error: {}", m_torrentFilePath, e.what()));
        return false;
    }
    m_isFileInited = true;
    return true;
}

void Torrent::handle(const std::vector<std::byte> &rawMessage, StreamSocketPtr streamSocket) {
    auto addr = streamSocket->getInetAddress();
    auto messages = BittorrentMessageParser::getMessages(rawMessage);
    if(messages.empty())
        return;

    for(auto& message: messages) {
        Logger::logInfo(std::format("handle {} - {}", BitTorrentMessageTypeConverter::messageTypeToString(static_cast<MessageType>(message.getMessageType())), streamSocket->getInetAddress().getHost()));
        switch (message.getMessageType()) {
            case MessageType::BITFIELD:
                bitfieldHandle(message, streamSocket);
                break;
            case MessageType::HAVE:
                haveHandle(message, streamSocket);
                break;
            case MessageType::PIECE:
                pieceHandle(message, streamSocket);
                break;
            default:
                break;
        }
    }
}

void Torrent::haveHandle(const BittorrentMessage &message, StreamSocketPtr streamSocket) {
    auto handleSocketHost = streamSocket->getInetAddress().getHost();
    auto peerIt = m_hostToPeer.find(streamSocket->getInetAddress().getHost());

    if(peerIt == m_hostToPeer.end())
        return;

    auto peerHost = peerIt->second.getSocket()->getInetAddress().getHost();
    if(peerHost == handleSocketHost) {
        peerIt->second.addPieces({static_cast<std::size_t>(message.getPayload().back())});
    }
}

void Torrent::bitfieldHandle(const BittorrentMessage &message, StreamSocketPtr streamSocket) {
    auto handleSocketHost = streamSocket->getInetAddress().getHost();
    auto peerIt = m_hostToPeer.find(streamSocket->getInetAddress().getHost());

    if(peerIt == m_hostToPeer.end())
        return;

    auto peerHost = peerIt->second.getSocket()->getInetAddress().getHost();
    if(peerHost == handleSocketHost) {
        auto pieces = BitfieldReader::getPiecesFromBitfield(message.getPayload());
        peerIt->second.addPieces(pieces);
    }
}

void Torrent::pieceHandle(const BittorrentMessage &message, StreamSocketPtr streamSocket) {
    m_files[0].writeData(message.getPayload());
}
