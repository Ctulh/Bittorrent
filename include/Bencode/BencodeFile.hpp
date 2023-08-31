#pragma once

#include "Bencode/BencodeParser.hpp"

#include <string>
#include <array>
#include <memory>

using TorrentHash = std::array<std::byte, 20>;

/**
 * @brief Class that represents file with torrent extension.
 */

class BencodeFile {
public:
/**
 * @param [in] path Path to the file with .torrent extension.
 * @throw WrongPathException If there is a non-existent directory in the filepath.
 * @throw NoSuchFileException If the file does not exist.
*/
    explicit BencodeFile(std::string const& path);
public:

/**
 * @return If 'announce' field exists - returns value of this field, or empty string if not.
*/
    [[nodiscard]] std::string getAnnounce() const;

/**
 * @return Sum of file sizes in a torrent file, or empty string if 'files' field doesn't exist.
*/
    [[nodiscard]] std::string getTotalBytesLeft() const;

/**
 * @return Array of size 20 and value: std::byte, which means sha1 on data with "info" fieldName.
*/
    [[nodiscard]] TorrentHash getInfoHash() const;

/**
 * @return vector of bencode strings, every string is element from 'files' field. If there is no 'file' field will return empty vector.
 */
    [[nodiscard]] std::vector<std::string> getFiles() const;

private:
    std::unique_ptr<BencodeParser> m_parser;
    std::string m_filepath;
};
