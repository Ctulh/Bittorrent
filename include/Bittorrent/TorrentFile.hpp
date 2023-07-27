#pragma once

#include <string>
#include <array>
#include <memory>

#include "Bencode/BencodeParser.hpp"

using TorrentHash = std::array<std::byte, 20>;

/**
 * @brief Class that represents file with torrent extension.
 */

class TorrentFile {
public:
    explicit TorrentFile(std::string const& path);

public:
    std::string getAnnounce();
    std::string getTotalBytesLeft();
    TorrentHash getInfoHash();

private:
    std::unique_ptr<BencodeParser> m_parser;
    std::string m_filepath;
};