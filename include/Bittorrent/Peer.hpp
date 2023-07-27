//
// Created by ctuh on 7/27/23.
//

#pragma once

#include <string>

class Peer {
public:
    Peer(std::string const& address, std::string const& port);

public:
    friend std::ostream& operator<<(std::ostream& os, Peer const& peer);

private:
    std::string m_address;
    std::string m_port;
};