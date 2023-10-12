//
// Created by ctuh on 10/10/23.
//

#pragma once

#include <vector>
#include <cinttypes>
#include <chrono>

enum class BlockStatus {
    FREE = 0,
    PENDING = 1,
    COMPLETE = 2,
};

uint32_t const BLOCK_SIZE = 16384;

using namespace std::chrono;
using TimePoint = std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<double>>;
std::chrono::sys_days const beginOfTheTime{1970y/1/1};

struct Block {
    Block(uint32_t blockSize): status(BlockStatus::FREE), lastUsedTimeStamp(beginOfTheTime), blockSize(blockSize) {}
    Block(Block const& block) = default;
    Block& operator=(Block const& block) = default;

public:
    void updateTimeStamp() {
        lastUsedTimeStamp = std::chrono::system_clock::now();
    }

public:
    std::vector<std::byte> rawData;
    BlockStatus status;
    TimePoint lastUsedTimeStamp;
    std::size_t blockSize;
};
