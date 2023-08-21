#pragma once

#include <string>
#include <array>
#include <memory>

#include "Bencode/BencodeParser.hpp"

using TorrentHash = std::array<std::byte, 20>;

/**
 * @brief Class that represents file with torrent extension.
 */

class BencodeFile {
public:
    explicit BencodeFile(std::string const& path);

public:
    std::string getAnnounce();
    std::string getTotalBytesLeft();
    std::vector<std::string> getFiles();
    TorrentHash getInfoHash();

private:
    std::unique_ptr<BencodeParser> m_parser;
    std::string m_filepath;
};