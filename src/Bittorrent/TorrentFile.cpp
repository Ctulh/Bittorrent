//
// Created by ctuh on 8/22/23.
//

#include <Bittorrent/TorrentFile.hpp>

TorrentFile::TorrentFile(std::string const& filePath, std::size_t size, std::size_t fileOffset): m_filePath(filePath),
                                                                                                 m_totalSize(size),
                                                                                                 m_fileStatus(TorrentFileStatus::NotDownloaded),
                                                                                                 m_bytesDownloaded(0),
                                                                                                 m_fileOffset(fileOffset) {
    m_fileStream.open(m_filePath, std::ios::binary | std::ios::out);
}

TorrentFile::~TorrentFile() {
    if(m_fileStream.is_open()) {
        m_fileStream.close();
    }
}


std::string TorrentFile::getFilePath() const {
    return m_filePath;
}

std::size_t TorrentFile::getSize() const {
    return m_totalSize;
}

TorrentFileStatus TorrentFile::getStatus() const {
    if(m_fileStatus == TorrentFileStatus::NotDownloaded && m_bytesDownloaded > 0 && m_bytesDownloaded < m_totalSize)
        m_fileStatus = TorrentFileStatus::PartiallyDownloaded;
    else if(m_fileStatus == TorrentFileStatus::NotDownloaded && m_bytesDownloaded == m_totalSize)
        m_fileStatus = TorrentFileStatus::Complete;
    return m_fileStatus;
}

void TorrentFile::writeData(std::vector<std::byte> data, std::size_t offset) { // TODO realize offset
    m_fileStream.write(reinterpret_cast<char*>(data.data()), static_cast<long>(data.size()));
    m_bytesDownloaded += data.size();
}

std::size_t TorrentFile::getBytesDownloaded() const {
    return m_bytesDownloaded;
}

std::size_t TorrentFile::getBytesLeft() const {
    return m_totalSize - m_bytesDownloaded;
}

std::size_t TorrentFile::getGlobalOffset() const {
    return m_fileOffset + m_bytesDownloaded;
}

std::size_t TorrentFile::getNextBlock() const {
    return m_fileOffset + m_bytesDownloaded;
}
