//
// Created by ctuh on 7/21/23.
//

#pragma once

#include "TorrentFile.hpp"

#include <string>

class TorrentFileBuilder {
public:
    static TorrentFile buildFromFile(std::string const& filepath);
};
