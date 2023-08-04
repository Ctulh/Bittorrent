//
// Created by ctuh on 7/27/23.
//

#include "PeerInfo.hpp"

PeerInfo::PeerInfo(std::string const& address, unsigned short port): m_address(address), m_port(port) {}

std::string PeerInfo::getAddress() const {
    return m_address;
}

unsigned short PeerInfo::getPort() const {
    return m_port;
}
