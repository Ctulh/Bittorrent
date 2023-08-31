#pragma once

#include "BencodeFile.hpp"

#include <string>

class TorrentFileBuilder {
public:
    static BencodeFile buildFromFile(std::string const& filepath);
};
