#pragma once

#include <vector>
#include "PeerInfo.hpp"
#include "Peer.hpp"

class Torrent {
public:

private:
    std::vector<Peer> m_peers;
};
