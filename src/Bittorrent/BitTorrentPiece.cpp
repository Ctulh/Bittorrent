//
// Created by ctuh on 10/10/23.
//
#include "BitTorrentPiece.hpp"

#include <fstream>

BitTorrentPiece::BitTorrentPiece(uint32_t pieceOffset, uint32_t pieceSize, uint32_t fileOffset, std::string filepath, uint32_t blockSize):
                                                                                                                        m_pieceOffset(pieceOffset),
                                                                                                                        m_pieceSize(pieceSize),
                                                                                                                        m_fileOffset(fileOffset),
                                                                                                                        m_blockSize(blockSize),
                                                                                                                        m_filepath(filepath),
                                                                                                                        m_fileWriter(filepath) {
    m_blocksAmount = m_pieceSize / blockSize;
    m_completeBlocksAmount = 0;
    m_blocks = std::vector<Block>(m_blocksAmount, Block(blockSize));
}

void BitTorrentPiece::writeBlock(uint32_t blockOffset, std::vector<std::byte> const& rawData) {
    if(blockOffset >= m_blocksAmount)
        return;

    uint32_t const dataOffset = (m_fileOffset * m_pieceSize) + blockOffset;
    auto& currBlock = m_blocks[blockOffset];

    currBlock.status = BlockStatus::COMPLETE;
    currBlock.rawData = rawData;
    m_completeBlocksAmount++;
}

void BitTorrentPiece::saveOnDisk() const {
    for(std::size_t i = 0; i < m_blocksAmount; ++i) {
        auto& currBlock = m_blocks[i];
        if(currBlock.status == BlockStatus::COMPLETE) {
            uint32_t fileOffset = (m_pieceOffset * m_pieceSize) + (i * m_blockSize);
            m_fileWriter.seek(fileOffset);
            m_fileWriter.write(currBlock.rawData.data(), currBlock.rawData.size());
        }
    }
}

std::vector<Block> const& BitTorrentPiece::getBlocks() const {
    return m_blocks;
}

std::vector<Block> BitTorrentPiece::getIncompleteBlocks() {
    std::vector<Block> output;

    for(auto const& block: m_blocks) {
        if(block.status != BlockStatus::COMPLETE) {
            output.push_back(block);
        }
    }
    return output;
}

bool BitTorrentPiece::isComplete() const {
    return m_completeBlocksAmount == m_blocksAmount;
}
