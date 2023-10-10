//
// Created by ctuh on 10/10/23.
//
#include "BitTorrentPiece.hpp"

#include <fstream>

void BitTorrentPiece::writeData(uint32_t blockOffset, std::vector<std::byte> const& rawData) {
    uint32_t const dataOffset = (m_fileOffset * m_pieceSize) + blockOffset;

    std::ofstream outputFile(m_filepath, std::ios::binary);
    outputFile.seekp(dataOffset);

    outputFile.write((char*)&rawData[0], rawData.size());
    outputFile.close();
}

std::vector<Block> const& BitTorrentPiece::getBlocks() const {
    return m_blocks;
}
