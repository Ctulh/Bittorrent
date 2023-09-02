#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "TorrentFileStatus.hpp"

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
    TorrentFile(std::string const& filePath, std::size_t size, std::size_t fileOffset = 0);
    TorrentFile(TorrentFile&& torrentFile)  noexcept = default;
    ~TorrentFile();

public:
/*!
 * @return path to the file with file name.
*/
    std::string getFilePath() const;

/*!
 * @return file size.
*/
    std::size_t getSize() const;

/*!
 * @return downloading status of the file.
*/
    TorrentFileStatus getStatus() const;

/*!
 * @param data - raw data to write in to the file.
 * @param offset - means where write 'data'.
*/
    void writeData(std::vector<std::byte> data, std::size_t offset = 0);

/*!
 * @return the wrote bytes of a file.
*/
    std::size_t getBytesDownloaded() const;

/*!
 * @return how many bytes are left to write for this file.
*/
    std::size_t getBytesLeft() const;

/*!
 * @return sum of files size before this file.
 */
    std::size_t getGlobalOffset() const;

private:
    std::ofstream m_fileStream;
    mutable TorrentFileStatus m_fileStatus;
    std::size_t m_totalSize;
    std::size_t m_bytesDownloaded;
    std::size_t m_fileOffset;
    std::string m_filePath;
};
