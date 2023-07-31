//
// Created by ctuh on 7/27/23.
//

#pragma once

#include <string>

class Peer {
public:
    Peer(std::string const& address, unsigned short port);

public:
    std::string getAddress() const;
    unsigned short getPort() const;

private:
    std::string m_address;
    unsigned short m_port;
};