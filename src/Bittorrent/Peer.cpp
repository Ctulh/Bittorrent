//
// Created by ctuh on 8/1/23.
//

#include "Peer.hpp"
#include "Logger/Logger.hpp"
#include "Inet/Socket.hpp"

Peer::Peer(PeerInfo const& peerInfo):   m_peerInfo(peerInfo),
                                        m_streamSocket(InetAddress(m_peerInfo.getAddress(), m_peerInfo.getPort())) {}

bool Peer::handshake(const std::string &request) {
    if(!m_streamSocket.setConnectTimeout(std::chrono::milliseconds(2000)))
        return false;
    if(!m_streamSocket.setReceiveTimeout(std::chrono::milliseconds(2000))) {
        return false;
    }
    //RAIISocketFlagsManipulator<SOL_SOCKET, SO_SNDTIMEO, timeval> flagsManipulator(m_socket.native_handle(), timeout);

    if(m_streamSocket.connect()) {
        if(m_streamSocket.send(request) == request.size()) {
            std::string response;
            m_streamSocket.receive(response);
            if(!response.empty()) {
                Logger::logInfo(std::format("Successful handshake with peer {}:{} Message: ", m_peerInfo.getAddress(), m_peerInfo.getPort(), response));
                return true;
            }
            else {
                Logger::logInfo(std::format("Error receiving response from peer {}:{}", m_peerInfo.getAddress(), m_peerInfo.getPort()));
            }
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


