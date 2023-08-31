//
// Created by ctuh on 7/27/23.
//

#pragma once

#include <string>

/*!
 * @brief сlass contains information about the remote peer
 */

class PeerInfo {
public:
    PeerInfo(std::string const& address, unsigned short port);
    PeerInfo(PeerInfo const& peerInfo) = default;
    PeerInfo& operator=(PeerInfo const& peerInfo) = default;

public:
    [[nodiscard]] std::string getAddress() const;
    [[nodiscard]] unsigned short getPort() const;

private:
    std::string m_address;
    unsigned short m_port;
};
