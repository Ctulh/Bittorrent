//
// Created by ctuh on 8/21/23.
//

#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "TorrentFileStatus.hpp"

class TorrentFile {
public:
    TorrentFile(std::string const& filePath, std::size_t size, std::size_t fileOffset = 0);
    ~TorrentFile();

public:
    std::string getFilePath() const;
    std::size_t getSize() const;
    TorrentFileStatus getStatus() const;
    void writeData(std::vector<std::byte> data, std::size_t offset = 0);
    std::size_t getBytesDownloaded() const;
    std::size_t getBytesLeft() const;
    std::size_t getGlobalOffset() const;

private:
    std::ofstream m_fileStream;
    mutable TorrentFileStatus m_fileStatus;
    std::size_t m_totalSize;
    std::size_t m_bytesDownloaded;
    std::size_t m_fileOffset;
    std::string m_filePath;
};