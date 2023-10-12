//
// Created by ctuh on 10/10/23.
//

#pragma once

#include <cinttypes>
#include <vector>
#include <string>

#include "Block.hpp"
#include "Utils/FileWriter.hpp"

class BitTorrentPiece {
public:
    BitTorrentPiece(uint32_t pieceIndex, uint32_t pieceSize, uint32_t pieceOffset, std::string filepath, uint32_t blockSize = BLOCK_SIZE);

public:
    void writeBlock(uint32_t blockOffset, std::vector<std::byte> const& rawData);
    void saveOnDisk() const;

    std::vector<Block> const& getBlocks() const;
    std::vector<Block> getIncompleteBlocks();
    bool isComplete() const;

private:
    std::vector<Block> generateBlocks() const;


private:
    uint32_t m_pieceOffset;
    uint32_t m_pieceSize;
    uint32_t m_fileOffset;
    uint32_t m_blockSize;
    std::vector<Block> m_blocks;
    std::string m_filepath;
    FileWriter m_fileWriter;
    uint32_t m_blocksAmount;
    uint32_t m_completeBlocksAmount;
};
