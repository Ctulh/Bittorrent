//
// Created by ctuh on 8/22/23.
//

#pragma once

#include "Utils/ByteMethods.hpp"

#include <gtest/gtest.h>

TEST(ByteMethodsTest, TestConvertZeroToFourBytes) {
    const std::vector<std::byte> expectedResult {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x0)};

    auto result = ByteMethods::convertNumberToFourBytes(0);

    EXPECT_EQ(result, expectedResult);
}

TEST(ByteMethodsTest, TestConvertNumberToFourBytesWithOnlyFourth) {
    const std::vector<std::byte> expectedResult {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0xFF)};

    auto result = ByteMethods::convertNumberToFourBytes(255);

    EXPECT_EQ(result, expectedResult);
}

TEST(ByteMethodsTest, TestConvertNumberToFourBytesWithOnlyThirdByte) {
    const std::vector<std::byte> expectedResult {std::byte(0x0), std::byte(0x0), std::byte(0xFF), std::byte(0x0)};

    auto result = ByteMethods::convertNumberToFourBytes(65280);

    EXPECT_EQ(result, expectedResult);
}

TEST(ByteMethodsTest, TestConvertNumberToFourBytesWithOnlySecondByte) {
    const std::vector<std::byte> expectedResult {std::byte(0x0), std::byte(0xFF), std::byte(0x0), std::byte(0x0)};

    auto result = ByteMethods::convertNumberToFourBytes(16711680);

    EXPECT_EQ(result, expectedResult);
}

TEST(ByteMethodsTest, TestConvertNumberToFourBytesWithOnlyFirstByte) {
    const std::vector<std::byte> expectedResult {std::byte(0xFF), std::byte(0x0), std::byte(0x0), std::byte(0x0)};

    auto result = ByteMethods::convertNumberToFourBytes(4278190080);

    EXPECT_EQ(result, expectedResult);
}

TEST(ByteMethodsTest, TestConvertNumberToFourBytesWithAllBytes) {
    const std::vector<std::byte> expectedResult {std::byte(0x01), std::byte(0x02), std::byte(0x03), std::byte(0x04)};

    auto result = ByteMethods::convertNumberToFourBytes(16909060);

    EXPECT_EQ(result, expectedResult);
}

TEST(ByteMethodsTest, TestConvertFourZeroBytesToNumber) {
    const uint32_t expectedResult = 0;
    const std::vector<std::byte> bytes {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x0)};

    auto result = ByteMethods::convertFourBytesToNumber(bytes);

    EXPECT_EQ(result, expectedResult);
}

TEST(ByteMethodsTest, TestConvertFourBytesToNumberWithFouthByte) {
    const uint32_t expectedResult = 255;
    const std::vector<std::byte> bytes {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0xFF)};

    auto result = ByteMethods::convertFourBytesToNumber(bytes);

    EXPECT_EQ(result, expectedResult);
}

TEST(ByteMethodsTest, TestConvertFourZeroBytesToNumberWithThirdByte) {
    const uint32_t expectedResult = 65280;
    const std::vector<std::byte> bytes {std::byte(0x0), std::byte(0x0), std::byte(0xFF), std::byte(0x0)};

    auto result = ByteMethods::convertFourBytesToNumber(bytes);

    EXPECT_EQ(result, expectedResult);
}

TEST(ByteMethodsTest, TestConvertFourZeroBytesToNumberWithSecondByte) {
    const uint32_t expectedResult = 16711680;
    const std::vector<std::byte> bytes {std::byte(0x0), std::byte(0xFF), std::byte(0x0), std::byte(0x0)};

    auto result = ByteMethods::convertFourBytesToNumber(bytes);

    EXPECT_EQ(result, expectedResult);
}

TEST(ByteMethodsTest, TestConvertFourZeroBytesToNumberWithFirstByte) {
    const uint32_t expectedResult = 4278190080;
    const std::vector<std::byte> bytes {std::byte(0xFF), std::byte(0x0), std::byte(0x0), std::byte(0x0)};

    auto result = ByteMethods::convertFourBytesToNumber(bytes);

    EXPECT_EQ(result, expectedResult);
}

TEST(ByteMethodsTest, TestConvertFourZeroBytesToNumberWithAllBytes) {
    const uint32_t expectedResult = 16909060;
    const std::vector<std::byte> bytes {std::byte(0x01), std::byte(0x02), std::byte(0x03), std::byte(0x04)};

    auto result = ByteMethods::convertFourBytesToNumber(bytes);

    EXPECT_EQ(result, expectedResult);
}