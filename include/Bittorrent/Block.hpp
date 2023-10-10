//
// Created by ctuh on 10/10/23.
//

#pragma once

#include <vector>
#include <cinttypes>

enum class BlockStatus {
    FREE = 0,
    PENDING = 1,
    COMPLETE = 2,
};

uint32_t const BLOCK_SIZE = 16384;

struct Block {
    Block(uint32_t blockSize): status(BlockStatus::FREE), lastUsedTimeStamp(0), blockSize(blockSize) {}

public:
    std::vector<std::byte> rawData;
    BlockStatus status;
    uint64_t lastUsedTimeStamp;
    std::size_t blockSize;
};
