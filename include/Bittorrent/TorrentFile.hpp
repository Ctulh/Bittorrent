#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "TorrentFileStatus.hpp"
#include "BitTorrentPiece.hpp"

/*!
 * @brief class represents file that should be downloaded.
 */

class TorrentFile {
public:
/*!
 * @param filePath - the final filePath from .torrent file.
 * @param size - the size of the file
 * @param fileOffset - sum of files size before this file
*/
    TorrentFile(std::string const& filePath, std::size_t size, std::size_t pieceOffset, std::size_t pieceSize);
    TorrentFile(TorrentFile&& torrentFile)  noexcept = default;
    ~TorrentFile() = default;

public:
/*!
 * @return path to the file with file name.
*/
    std::string getFilePath() const;

/*!
 * @return downloading status of the file.
*/
    TorrentFileStatus getStatus() const;

/*!
 * @param data - raw data to write in to the file.
 * @param offset - means where write 'data'.
*/
    void writeData(std::vector<std::byte> data, std::size_t offset = 0);

    std::vector<BitTorrentPiece> const& getPieces() const;

private:
    std::vector<BitTorrentPiece> createPieces();

private:
    std::vector<BitTorrentPiece> m_pieces;
    mutable TorrentFileStatus m_fileStatus;
    std::string m_filePath;
    std::size_t m_pieceOffset;
    std::size_t m_fileSize;
    std::size_t m_pieceSize;
};
