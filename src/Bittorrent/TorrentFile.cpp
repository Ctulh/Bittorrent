/**
 * @file
 * @brief Implementation of the class TorrentFile
 */

#include "TorrentFile.hpp"
#include "FileReader/FileReader.hpp"
#include "Utils/HashMethods.hpp"
#include "Bencode/Exceptions/NoSuchFieldException.hpp"
#include "Logger/Logger.hpp"

#include <format>

/**
 * @param [in] path Path to the file with .torrent extension.
 * @throw WrongPathException If there is a non-existent directory in the filepath.
 * @throw NoSuchFileException If the file does not exist.
 */
TorrentFile::TorrentFile(std::string const& filepath): m_filepath(filepath) {
    FileReader fileReader(filepath);
    std::string fileData = fileReader.getData();
    m_parser = std::make_unique<BencodeParser>(fileData);
}

/**
 * @return Sum of file sizes in a torrent file.
 */
std::string TorrentFile::getTotalBytesLeft() {
    uint64_t bytesLength = 0;
    StringVector torrentFiles;

    try {
        torrentFiles = m_parser->getList("files");
    } catch(NoSuchFieldException& exception) {
        Logger::logError(std::format("Caught an exception in TorrentFile::getTotalBytesLength. Filepath: {} "
                                     "Exception message: {}", m_filepath, exception.what()));
        return "";
    }

    for(auto const& file: torrentFiles) {
        BencodeParser parser(file);
        std::string fileBytesLength = parser.getValue("length");
        bytesLength += std::stoull(fileBytesLength);
    }

    return std::to_string(bytesLength);
}

/**
 * @return Value with "announce" fieldName.
 */
std::string TorrentFile::getAnnounce() {
    std::string announceString;
    try {
        announceString = m_parser->getValue("announce");
    } catch(NoSuchFieldException& exception) {
        Logger::logError(std::format("Caught an exception in TorrentFile::getAnnounce. Filepath: {} "
                                     "Exception message: {}", m_filepath, exception.what()));
    }
    return announceString;
}

/**
 *
 * @return Array of size 20 and value: std::byte, which means sha1 on data with "info" fieldName.
 */
TorrentHash TorrentFile::getInfoHash() {
    std::string infoRawData;
    try {
        infoRawData = m_parser->getValue("info");
    } catch(NoSuchFieldException& exception) {
        Logger::logError(std::format("Caught an exception in TorrentFile::getInfoHash. Filepath: {} "
                                     "Exception message: {}", m_filepath, exception.what()));
    }
    auto result = hash::sha1(infoRawData);
    return result;
}
