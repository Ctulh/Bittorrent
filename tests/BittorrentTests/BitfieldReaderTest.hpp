//
// Created by ctuh on 9/2/23.
//

#pragma once

#include <gtest/gtest.h>

#include "Bittorrent/BittorrentMessages/BitfieldReader.hpp"

TEST(BitfieldReaderTest, TestReadEmptyBitfield) {
    const std::vector<std::byte> bitfield {};

    const auto result = BitfieldReader::getPiecesFromBitfield(bitfield);

    EXPECT_TRUE(result.empty());
}

TEST(BitfieldReaderTest, TestReadBitfieldWithOneZeroByte) {
    const std::vector<std::byte> bitfield {std::byte(0x0)};

    const auto result = BitfieldReader::getPiecesFromBitfield(bitfield);

    EXPECT_TRUE(result.empty());
}

TEST(BitfieldReaderTest, TestReadBitfieldWithOneByte) {
    const std::vector<std::byte> bitfield {std::byte(0x1)};

    const auto result = BitfieldReader::getPiecesFromBitfield(bitfield);

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 7);
}

TEST(BitfieldReaderTest, TestReadBitfieldWithFewBytes) {
    const std::vector<std::byte> bitfield {std::byte(0xAA), std::byte(0x55)};
    const std::vector<std::size_t> expectedResult = {0,2,4,6,9,11,13,15};

    const auto result = BitfieldReader::getPiecesFromBitfield(bitfield);

    EXPECT_EQ(result, expectedResult);
}
