//
// Created by ctuh on 8/1/23.
//

#include "Peer.hpp"
#include "Logger/Logger.hpp"
#include "Inet/Socket.hpp"
#include <algorithm>

Peer Peer::fromAddressAndPort(const std::string &address, unsigned short port) {
    return Peer(address, port);
}

Peer::Peer(std::string const& address, unsigned short port): m_status(PeerStatus::NONE), m_address(address), m_port(port) {
    m_streamSocket = std::make_shared<StreamSocket>(InetAddress(address, port));
}

bool Peer::handshake(const std::string &request) const {
    if(!m_streamSocket->setConnectTimeout(std::chrono::milliseconds(2000)))
        return false;
    if(!m_streamSocket->setReceiveTimeout(std::chrono::milliseconds(2000))) {
        return false;
    }

    if(m_streamSocket->connect()) {
        if(m_streamSocket->send(request) == request.size()) {
            Logger::logInfo(std::format("Successful handshake with peer {}:{}", m_address, m_port));
            return true;
        }
        else {
            Logger::logInfo(std::format("Error sending request to peer {}:{}", m_address, m_port));
        }
    }
    else {
        Logger::logInfo(std::format("Error connecting to peer {}:{}", m_address, m_port));
    }
    return false;
}

void Peer::addPieces(const std::vector<std::size_t> &pieces) {
    std::copy(pieces.begin(), pieces.end(), std::inserter(m_pieces, m_pieces.end()));;
}

bool Peer::hasPiece(std::size_t piece) const {
    return m_pieces.contains(piece);
}

void Peer::setStatus(PeerStatus status) {
    m_status = status;
}

StreamSocketPtr Peer::getSocket() const {
    return m_streamSocket;
}

PeerStatus Peer::getStatus() const {
    return m_status;
}

std::string Peer::getAddress() const {
    return m_address;
}

unsigned short Peer::getPort() const {
    return m_port;
}
