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
        this->callback(rawMessage, streamSocket);
    });
}

void Torrent::run() {
    if(!initFiles())
        return;
    if(!initPeers()) {
        return;
    }

    while(m_files[0].getStatus() != TorrentFileStatus::Complete) {
        auto& file = m_files[0];
        auto nextPiece = file.getNextBlock();

        Request request(nextPiece, 0, m_pieceLength);

        for(auto&& [host, peer]: m_hostToPeer) {
            if(peer.hasPiece(nextPiece)) {
                peer.getSocket()->send(request.getMessage());
            }
        }
        m_poller->poll();
        int number;
        std::cin >> number;
        if(number != 1) {
            exit(0);
        }
    }
}

void Torrent::stop() {

}

bool Torrent::initPeers() {
    try {
        BencodeFile bencodeFile(m_torrentFilePath);
        std::string announce = bencodeFile.getAnnounce();
        auto request = RequestBuilder::buildTrackerRequest(bencodeFile, UrlMethods::getDomainName(announce));
        auto response = RequestSender::sendRequest<TrackerResponse>(request, UrlMethods::getDomainName(announce), "80");

        auto torrentHash = bencodeFile.getInfoHash();
        std::string infoHash;
        for (auto hashEl: torrentHash) {
            infoHash.push_back(static_cast<char>(hashEl));
        }

        HandshakeMessageBuilder builder;
        builder.setInfoHash(infoHash);

        m_handshake = builder.getMessage();


        auto peerInfos = response.getPeers();
        auto totalPeers = peerInfos.size();


        for (auto &peerInfo: peerInfos) {
            Peer peer(peerInfo);
            if(peer.handshake(m_handshake)) {
                m_poller->add(peer.getSocket());
                m_hostToPeer.emplace(peerInfo.getAddress(), peer);
            }
        }
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

void Torrent::callback(const std::vector<std::byte> &rawMessage, StreamSocketPtr streamSocket) {
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
            default:
                std::cout;
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