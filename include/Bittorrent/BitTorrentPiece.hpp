//
// Created by ctuh on 10/10/23.
//

#pragma once

#include <cinttypes>
#include <vector>
#include <string>

#include "Block.hpp"

class BitTorrentPiece {
public:
    BitTorrentPiece(uint32_t pieceIndex, uint32_t pieceSize, uint32_t fileOffset, std::string filepath, uint32_t blockSize = BLOCK_SIZE): m_pieceIndex(pieceIndex), m_pieceSize(pieceSize), m_fileOffset(fileOffset), m_blockSize(blockSize), m_filepath(filepath) {}

public:
    void writeData(uint32_t blockOffset, std::vector<std::byte> const& rawData);
    std::vector<Block> const& getBlocks() const;

private:
    std::vector<Block> generateBlocks() const;


private:
    uint32_t m_pieceIndex;
    uint32_t m_pieceSize;
    uint32_t m_fileOffset;
    uint32_t m_blockSize;
    std::vector<Block> m_blocks;
    std::string m_filepath;
};
