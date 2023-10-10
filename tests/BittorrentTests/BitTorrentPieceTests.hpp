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
    }
    void TearDown() override {
        if(std::filesystem::exists(filepath)) {
            std::filesystem::remove(filepath);
        }

    }
public:
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

TEST_F(BitTorrentPieceFixture, CreateFileOnWriteData) {
    uint32_t const pieceIndex = 0;
    uint32_t const pieceSize = 4;
    uint32_t const fileOffset = 0;
    uint32_t const blockSize = 1;

    piece = std::make_unique<BitTorrentPiece>(pieceIndex, pieceSize, fileOffset, filepath, blockSize);

    std::vector<std::byte> const data {std::byte(0x1)};

    EXPECT_FALSE(std::filesystem::exists(filepath));
    EXPECT_NO_THROW(piece->writeData(0, data));
    EXPECT_TRUE(std::filesystem::exists(filepath));
}

TEST_F(BitTorrentPieceFixture, WriteData) {
    uint32_t const pieceIndex = 0;
    uint32_t const pieceSize = 4;
    uint32_t const fileOffset = 0;
    uint32_t const blockSize = 1;

    piece = std::make_unique<BitTorrentPiece>(pieceIndex, pieceSize, fileOffset, filepath, blockSize);

    std::vector<std::byte> const data {std::byte(0x1)};
    piece->writeData(0, data);

    auto dataFromFile = readBinaryFile(filepath);

    EXPECT_FALSE(dataFromFile.empty());
    EXPECT_EQ(dataFromFile[0], data[0]);
}

TEST_F(BitTorrentPieceFixture, AppendTextToPiece) {

}
