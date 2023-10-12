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
#include "BittorrentMessages/BitfieldReader.hpp"

Torrent::Torrent(const std::string &filepath): m_messageHandler(std::make_unique<MessageHandler>()),
                                               m_torrentFilePath(filepath),
                                               m_poller(std::make_unique<SocketPoller>(10,10000)),
                                               m_timer(std::make_unique<Timer>()){
    m_poller->setReceiveMessageCallback([this](InetAddress const& address,std::vector<std::byte> const& rawMessage) {
        this->handle(address.getHost(), rawMessage);
    });

    m_messageHandler->setHandler(MessageType::BITFIELD, [this](std::string const& host, std::vector<std::byte> const& rawMessage) {
        bitfieldHandle(host, rawMessage);
    });
    m_messageHandler->setHandler(MessageType::UNCHOKE, [this](std::string const& host, std::vector<std::byte> const& rawMessage) {
        unchokeHandle(host, rawMessage);
    });
    m_messageHandler->setHandler(MessageType::HAVE, [this](std::string const& host, std::vector<std::byte> const& rawMessage) {
        haveHandle(host, rawMessage);
    });
    m_messageHandler->setHandler(MessageType::CHOKE, [this](std::string const& host, std::vector<std::byte> const& rawMessage) {
        chokeHandle(host, rawMessage);
    });
}

void Torrent::addFilePieceToQueue(TorrentFile const& file) {
    //auto index = (file.getGlobalOffset() + file.getBytesDownloaded()) / m_pieceLength;
  //  Task task(index, file.getNextBlock());
  //  m_taskQueue.push(task);
}

Torrent::~Torrent() {
    if(m_producerThread && m_producerThread->joinable()) {
        m_producerThread->join();
    }
    if(m_consumerThread && m_consumerThread->joinable()) {
        m_consumerThread->join();
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
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

void Torrent::consumeTasks() {
    if(m_taskQueue.empty())
        return;
    auto task = m_taskQueue.front();
    m_taskQueue.pop();

    for(auto&& [host, peer]: m_hostToPeer) {
        if(peer.hasPiece(task.piece)) {
            if(peer.getStatus() == PeerStatus::NONE) {
                Interested interested;
                peer.getSocket()->send(interested.getMessage());
                continue;
            } else if(peer.getStatus() == PeerStatus::Unchoked) {
                Request request(task.piece, task.begin);
                peer.getSocket()->send(request.getMessage());
                return;
            }
        }
    }

}

void Torrent::run() {
    m_isRunning.test_and_set();
    if(!initFiles())
        return;
    if(!initPeers())
        return;
    m_producerThread = std::make_unique<std::thread>([this]() {
       while(m_isRunning.test()) {
           produceTasks();
       }
    });

    m_consumerThread = std::make_unique<std::thread>([this]() {
        while(m_isRunning.test()) {
            consumeTasks();
        }
    });

    m_poller->poll();

    std::this_thread::sleep_for(std::chrono::seconds(100));
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


bool Torrent::initPeers() {
    if(m_timer && not m_timer->isExpired())
        return true;
    try {
        auto trackerResponse = sendTrackerRequest();
        auto handshakeMessage = createHandshakeMessage();
        addPeerIfConfirmHandshake(trackerResponse.getPeers(), handshakeMessage);
        m_timer->setTimeout(std::chrono::seconds(trackerResponse.getInterval()));
    }
    catch(std::exception& e) {
        Logger::logError(std::format("Error in Torrent::initPeers() for file: {} Error: {}", m_torrentFilePath, e.what()));
        return false;
    }
    return true;
}

bool Torrent::initFiles() {
    if(!m_files.empty())
        return true;
    try {
        BencodeFile bencodeFile(m_torrentFilePath);

        auto files = bencodeFile.getFiles();

        std::size_t offset = 0;
        for (auto file: files) {
            BencodeParser fileParser(file);
            std::size_t fileSize = std::stoi(fileParser["length"]);
            std::string filePath = fileParser["path"];
//m_files.emplace_back(filePath, fileSize, offset);
            offset += fileSize;
        }

        m_pieceLength = std::stoi(bencodeFile.getPieceLength());
    }
    catch(std::exception& e) {
        Logger::logError(std::format("Error in Torrent::initFiles() for file: {} Error: {}", m_torrentFilePath, e.what()));
        return false;
    }
    return true;
}

void Torrent::handle(std::string const& host, const std::vector<std::byte> &rawMessage) {
    auto messages = BittorrentMessageParser::getMessages(rawMessage);
    if(messages.empty())
        return;

    for(auto& message: messages) {
        Logger::logInfo(std::format("handle {} - {}", BitTorrentMessageTypeConverter::messageTypeToString(static_cast<MessageType>(message.getMessageType())), host));
        m_messageHandler->handle(host, message);
    }
}

void Torrent::chokeHandle(const std::string &host, const std::vector<std::byte> &payload) {
    auto it = m_hostToPeer.find(host);
    if(it == m_hostToPeer.end()) {
        return;
    }
    it->second.setStatus(PeerStatus::Choked);
}

void Torrent::unchokeHandle(const std::string &host, const std::vector<std::byte> &payload) {
    auto it = m_hostToPeer.find(host);
    if(it == m_hostToPeer.end()) {
        return;
    }
    it->second.setStatus(PeerStatus::Unchoked);
}

void Torrent::interestedHandle(std::string const& host, std::vector<std::byte> const& payload) {
}

void Torrent::notInterestedHandle(std::string const& host, std::vector<std::byte> const& payload) {

}

void Torrent::haveHandle(std::string const& host, std::vector<std::byte> const& payload) {
    auto it = m_hostToPeer.find(host);

    if(it == m_hostToPeer.end())
        return;

    uint32_t piece = ByteMethods::convertFourBytesToNumber({payload.end() - 4, payload.end()});
    it->second.addPieces({piece});
}

void Torrent::bitfieldHandle(std::string const& host, std::vector<std::byte> const& payload) {
    auto it = m_hostToPeer.find(host);

    if(it == m_hostToPeer.end())
        return;

    auto pieces = BitfieldReader::getPiecesFromBitfield(payload);
    it->second.addPieces(pieces);
}

void Torrent::requestHandle(std::string const& host, std::vector<std::byte> const& payload) {

}

void Torrent::pieceHandle(std::string const& host, std::vector<std::byte> const& payload) {

}

void Torrent::cancelHandle(std::string const& host, std::vector<std::byte> const& payload) {

}

void Torrent::portHandle(std::string const& host, std::vector<std::byte> const& payload) {

}