//
// Created by ctuh on 8/22/23.
//

#include <Bittorrent/TorrentFile.hpp>

TorrentFile::TorrentFile(std::string const& filePath, std::size_t size, std::size_t pieceOffset, std::size_t pieceSize): m_filePath(filePath),
                                                                                                 m_fileSize(size),
                                                                                                 m_fileStatus(TorrentFileStatus::NotDownloaded),
                                                                                                 m_pieceOffset(pieceOffset),
                                                                                                 m_pieceSize(pieceSize) {
    m_pieces = createPieces();
}

std::string TorrentFile::getFilePath() const {
    return m_filePath;
}

TorrentFileStatus TorrentFile::getStatus() const {
    return m_fileStatus;
}

void TorrentFile::writeData(std::vector<std::byte> data, std::size_t offset) { // TODO realize offset

}

std::vector<BitTorrentPiece> const& TorrentFile::getPieces() const {
    return m_pieces;
}

std::vector<BitTorrentPiece> TorrentFile::createPieces() {
    std::size_t const pieceAmount = m_fileSize / m_pieceSize;
    std::vector<BitTorrentPiece> output;
    output.reserve(pieceAmount);

    for(std::size_t i = 0; i < pieceAmount; ++i) {
        output.emplace_back(m_pieceOffset + i, m_pieceSize, i, m_filePath);
    }
    return output;
}
