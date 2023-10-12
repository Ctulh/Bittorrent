//
// Created by ctuh on 10/10/23.
//

#pragma once

#include <filesystem>
#include <iterator>
#include <gtest/gtest.h>

#include "Bittorrent/BitTorrentPiece.hpp"

class BitTorrentPieceFixture: public testing::Test {
public:
    void SetUp() override {
        piece = std::make_unique<BitTorrentPiece>(pieceIndex, pieceSize, fileOffset, filepath, blockSize);
    }
    void TearDown() override {
        if(std::filesystem::exists(filepath)) {
            std::filesystem::remove(filepath);
        }

    }
public:
    uint32_t const pieceIndex = 0;
    uint32_t const pieceSize = 4;
    uint32_t const fileOffset = 0;
    uint32_t const blockSize = 1;
    std::string filepath = "BitTorrentPieceTest.txt";
    std::unique_ptr<BitTorrentPiece> piece;
};

std::vector<std::byte> readBinaryFile(std::string const& filepath) {
    if(!std::filesystem::exists(filepath))
        return {};

    std::ifstream file(filepath, std::ios::binary);
    file.seekg(0, std::ios::end);
    std::size_t const fileSize = file.tellg();

    file.seekg(0, std::ios::beg);

    std::vector<std::byte> data(fileSize, std::byte(0x0));
    file.read(reinterpret_cast<char*>(&data[0]), fileSize);
    return data;
}


TEST(BitTorrentPieceTest, TestConstructBitTorrentPiece) {
    uint32_t pieceIndex = 0;
    uint32_t pieceSize = 0;
    uint32_t fileOffset = 0;
    std::string const filepath = "BitTorrentPieceTest.txt";

    EXPECT_NO_THROW(BitTorrentPiece piece(pieceIndex, pieceSize, fileOffset, filepath));
}

TEST_F(BitTorrentPieceFixture, GetBlocks) {
    auto blocks = piece->getBlocks();

    EXPECT_EQ(blocks.size(), pieceSize / blockSize);

    for(auto& block: blocks) {
        EXPECT_EQ(block.blockSize, blockSize);
    }
}

TEST_F(BitTorrentPieceFixture, writeBlock) {
    std::vector<std::byte> data {0x1};
    uint32_t const blockOffset = 0;

    EXPECT_NO_THROW(piece->writeBlock(blockOffset, data));

    auto blocks = piece->getBlocks();

    EXPECT_EQ(blocks.size(), pieceSize / blockSize);
    EXPECT_EQ(blocks[blockOffset].rawData.size(), 1);
    EXPECT_EQ(blocks[blockOffset].rawData[0], data[0]);
    EXPECT_EQ(blocks[blockOffset].status, BlockStatus::COMPLETE);

    for(std::size_t i = 0; i < blocks.size(); ++i) {
        if(i == blockOffset)
            continue;
        EXPECT_EQ(blocks[i].blockSize, blockSize);
        EXPECT_EQ(blocks[i].status, BlockStatus::FREE);
        EXPECT_TRUE(blocks[i].rawData.empty());
    }
}

TEST_F(BitTorrentPieceFixture, WriteToMultipleBlocks) {
    std::vector<std::vector<std::byte>> data{{std::byte(0x1)}, {std::byte(0x2)}, {std::byte(0x3)}, {std::byte(0x4)}};

    for(std::size_t i = 0; i < data.size(); ++i) {
        piece->writeBlock(i, data[i]);
    }

    auto blocs = piece->getBlocks();

    for(std::size_t i = 0; i < data.size(); ++i) {
        EXPECT_EQ(blocs[i].rawData, data[i]);
        EXPECT_EQ(blocs[i].status, BlockStatus::COMPLETE);
    }
}

TEST_F(BitTorrentPieceFixture, GetIncompleteBlocksWhenAllBlocksIncomplete) {
    auto incompleteBlocks = piece->getIncompleteBlocks();

    EXPECT_EQ(incompleteBlocks.size(), pieceSize / blockSize);
}

TEST_F(BitTorrentPieceFixture, GetIncompleteBlocksWhenPieceHaveOneIncompleteBlock) {
    std::vector<std::byte> data {std::byte(0x1)};

    piece->writeBlock(0, data);

    auto incompleteBlocks = piece->getIncompleteBlocks();

    EXPECT_EQ(incompleteBlocks.size(), (pieceSize / blockSize) - 1);
}

TEST_F(BitTorrentPieceFixture, GetIncompleteBlocksWhenAllBlocksAreComplete) {
    std::vector<std::byte> data {std::byte(0x1)};

    for(std::size_t i = 0; i < pieceSize / blockSize; ++i) {
        piece->writeBlock(i, data);
    }

    auto incompleteBlocks = piece->getIncompleteBlocks();
    EXPECT_TRUE(incompleteBlocks.empty());
}

TEST(BitTorrentPieceTest, SaveOnDiskWithoutCompleteBlocks) {
    uint32_t const pieceIndex = 0;
    uint32_t const pieceSize = 4;
    uint32_t const fileOffset = 0;
    uint32_t const blockSize = 1;
    std::string filepath = "BitTorrentPieceTest.txt";

    BitTorrentPiece piece(pieceIndex, pieceSize, fileOffset, filepath, blockSize);

    EXPECT_NO_THROW(piece.saveOnDisk());

    auto fileData = readBinaryFile(filepath);
    EXPECT_TRUE(fileData.empty());
}

TEST(BitTorrentPieceTest, SaveOnDiskWithOneCompleteBlock) {
    uint32_t const pieceIndex = 0;
    uint32_t const pieceSize = 4;
    uint32_t const fileOffset = 0;
    uint32_t const blockSize = 1;
    std::string filepath = "BitTorrentPieceTest.txt";

    BitTorrentPiece piece(pieceIndex, pieceSize, fileOffset, filepath, blockSize);

    std::vector<std::byte> data {std::byte(0x1)};
    piece.writeBlock(0, data);

    EXPECT_NO_THROW(piece.saveOnDisk());

    auto fileData = readBinaryFile(filepath);
    EXPECT_EQ(fileData.size(), 1);
    EXPECT_EQ(fileData, data);
}

TEST(BitTorrentPieceTest, SaveOnDiskWithMultipleCompleteBlocks) {
    uint32_t const pieceIndex = 0;
    uint32_t const pieceSize = 4;
    uint32_t const fileOffset = 0;
    uint32_t const blockSize = 1;
    std::string filepath = "BitTorrentPieceTest.txt";

    BitTorrentPiece piece(pieceIndex, pieceSize, fileOffset, filepath, blockSize);

    std::vector<std::vector<std::byte>> data{{std::byte(0x1)}, {std::byte(0x2)}, {std::byte(0x3)}, {std::byte(0x4)}};
    for(std::size_t i = 0; i < data.size(); ++i) {
        piece.writeBlock(i, data[i]);
    }

    EXPECT_NO_THROW(piece.saveOnDisk());

    auto fileData = readBinaryFile(filepath);
    EXPECT_EQ(fileData.size(), data.size());

    for(std::size_t i = 0; i < data.size(); ++i) {
        EXPECT_EQ(fileData[i], data[i][0]);
    }
}

TEST(BitTorrentPieceTest, SaveOnDiskWithMultipleCompleteBlocksInNonSequentialOrder) {
    uint32_t const pieceIndex = 0;
    uint32_t const pieceSize = 4;
    uint32_t const fileOffset = 0;
    uint32_t const blockSize = 1;
    std::string filepath = "BitTorrentPieceTest.txt";

    BitTorrentPiece piece(pieceIndex, pieceSize, fileOffset, filepath, blockSize);

    std::vector<std::vector<std::byte>> data{{std::byte(0x1)}, {std::byte(0x2)}, {std::byte(0x3)}, {std::byte(0x4)}};
    for(int i = data.size() - 1; i >= 0; --i) {
        piece.writeBlock(i, data[3 - i]);
    }

    EXPECT_NO_THROW(piece.saveOnDisk());

    auto fileData = readBinaryFile(filepath);
    EXPECT_EQ(fileData.size(), data.size());

    for(int i = data.size() - 1; i >= 0; --i) {
        EXPECT_EQ(fileData[i], data[3 - i][0]);
    }
}
