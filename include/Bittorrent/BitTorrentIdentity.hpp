#pragma once

#include <string>

class BitTorrentIdentity {
public:
    BitTorrentIdentity() = delete;

public:
    static std::string getPeerId();
};
