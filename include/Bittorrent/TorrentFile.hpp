//
// Created by ctuh on 8/21/23.
//

#pragma once

#include <string>

class TorrentFile {
public:
    TorrentFile(std::string const& filePath, std::size_t size);

private:
    std::size_t m_size;
    std::string m_filePath;
};