//
// Created by ctuh on 7/21/23.
//

#pragma once

#include <string>
#include <array>
#include <vector>

using TorrentHash = std::array<std::byte, 20>;

class TorrentFile {
public:
private:
    std::string m_announce;
    std::string m_name;
    std::string m_pieces;
    TorrentHash m_infoHash;
    std::vector<TorrentHash> pieceHashes;
    int m_pieceLength;
    int m_length;
};