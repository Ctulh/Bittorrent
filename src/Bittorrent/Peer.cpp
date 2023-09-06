//
// Created by ctuh on 8/1/23.
//

#include "Peer.hpp"
#include "Logger/Logger.hpp"
#include "Inet/Socket.hpp"
#include <algorithm>

Peer::Peer(PeerInfo const& peerInfo):   m_peerInfo(peerInfo) {
    m_streamSocket = std::make_shared<StreamSocket>(InetAddress(m_peerInfo.getAddress(), m_peerInfo.getPort()));
}

bool Peer::handshake(const std::string &request) {
    if(!m_streamSocket->setConnectTimeout(std::chrono::milliseconds(2000)))
        return false;
    if(!m_streamSocket->setReceiveTimeout(std::chrono::milliseconds(2000))) {
        return false;
    }

    if(m_streamSocket->connect()) {
        if(m_streamSocket->send(request) == request.size()) {
            Logger::logInfo(std::format("Successful handshake with peer {}:{}", m_peerInfo.getAddress(), m_peerInfo.getPort()));
            return true;
        }
        else {
            Logger::logInfo(std::format("Error sending request to peer {}:{}", m_peerInfo.getAddress(), m_peerInfo.getPort()));
        }
    }
    else {
        Logger::logInfo(std::format("Error connecting to peer {}:{}", m_peerInfo.getAddress(), m_peerInfo.getPort()));
    }
    return false;
}

void Peer::addPieces(const std::vector<std::size_t> &pieces) {
    std::copy(pieces.begin(), pieces.end(), std::inserter(m_pieces, m_pieces.end()));;
}

bool Peer::hasPiece(std::size_t piece) const {
    return m_pieces.contains(piece);
}
