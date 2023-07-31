//
// Created by ctuh on 7/27/23.
//

#include "Peer.hpp"

Peer::Peer(const std::string &address, unsigned short port): m_address(address), m_port(port) {}

std::string Peer::getAddress() const {
    return m_address;
}

unsigned short Peer::getPort() const {
    return m_port;
}
