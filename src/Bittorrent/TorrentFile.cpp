
#include "TorrentFile.hpp"
#include "FileReader/FileReader.hpp"
#include "Utils/HashMethods.hpp"

TorrentFile::TorrentFile(const std::string &path) {
    FileReader fileReader(path);
    std::string fileData = fileReader.getData();
    m_parser = std::make_unique<BencodeParser>(fileData);
}

std::string TorrentFile::getTotalBytesLeft() {
    uint64_t bytesLength = 0;

    auto torrentFiles = m_parser->getList("files");
    for(auto const& file: torrentFiles) {
        BencodeParser parser(file);
        std::string fileBytesLength = parser.getValue("length");
        bytesLength += std::stoull(fileBytesLength);
    }

    return std::to_string(bytesLength);
}

std::string TorrentFile::getAnnounce() {
    return m_parser->getValue("announce");
}

TorrentHash TorrentFile::getInfoHash() {
    std::string infoRawData = m_parser->getValue("info");
    auto result = hash::sha1(infoRawData);
    return result;
}
