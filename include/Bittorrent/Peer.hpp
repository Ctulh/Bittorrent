//
// Created by ctuh on 8/1/23.
//

#pragma once

#include "PeerInfo.hpp"
#include "Socket.hpp"
#include "StreamSocket.hpp"

#include <boost/asio.hpp>
#include <memory>

class Peer {
public:
    Peer(PeerInfo const& peerInfo);
public:
    bool handshake(std::string const& request);
    StreamSocketPtr getSocket() {
        return m_streamSocket;
    }
private:
    PeerInfo m_peerInfo;
    StreamSocketPtr m_streamSocket;
};