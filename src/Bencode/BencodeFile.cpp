/**
 * @file
 * @brief Implementation of the class BencodeFile
 */

#include "BencodeFile.hpp"
#include "FileReader/FileReader.hpp"
#include "Utils/HashMethods.hpp"
#include "Bencode/Exceptions/NoSuchFieldException.hpp"
#include "Logger/Logger.hpp"

#include <format>

BencodeFile::BencodeFile(std::string const& filepath): m_filepath(filepath) {
    FileReader fileReader(filepath);
    std::string fileData = fileReader.getData();
    m_parser = std::make_unique<BencodeParser>(fileData);
}

std::string BencodeFile::getTotalBytesLeft() const {
    uint64_t bytesLength = 0;
    StringVector torrentFiles;

    try {
        torrentFiles = m_parser->getList("files");
    } catch(NoSuchFieldException& exception) {
        Logger::logError(std::format("Caught an exception in BencodeFile::getTotalBytesLength. Filepath: {} "
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

std::string BencodeFile::getAnnounce() const {
    std::string announceString;
    try {
        announceString = m_parser->getValue("announce");
    } catch(NoSuchFieldException& exception) {
        Logger::logError(std::format("Caught an exception in BencodeFile::getAnnounce. Filepath: {} "
                                     "Exception message: {}", m_filepath, exception.what()));
    }
    return announceString;
}

TorrentHash BencodeFile::getInfoHash() const {
    std::string infoRawData;
    try {
        infoRawData = m_parser->getValue("info");
    } catch(NoSuchFieldException& exception) {
        Logger::logError(std::format("Caught an exception in BencodeFile::getInfoHash. Filepath: {} "
                                     "Exception message: {}", m_filepath, exception.what()));
    }
    auto result = hash::sha1(infoRawData);
    return result;
}

std::vector<std::string> BencodeFile::getFiles() const {
    std::vector<std::string> files;
    try {
        files = m_parser->getList("files");
    }
    catch(NoSuchFieldException& exception) {
        Logger::logError(std::format("Caught an exception in BencodeFile::getInfoHash. Filepath: {} "
                                     "Exception message: {}", m_filepath, exception.what()));
    }
    return files;
}

std::string BencodeFile::getPieceLength() const {
    return m_parser->getValue("piece length");
}


