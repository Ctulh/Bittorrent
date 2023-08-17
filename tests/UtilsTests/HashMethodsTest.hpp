
#pragma once

#include "Utils/HashMethods.hpp"

#include <gtest/gtest.h>
#include <array>
#include <numeric>

TEST(HashMethodsTest, TestUrlEncodeWithLowerCaseAlphabet) {
    const std::size_t alphabetSize = 26;
    std::array<std::byte, 20> lowerCaseAlphabetFirstSymbols{};
    std::array<std::byte, 20> lowerCaseAlphabetLastSymbols{};
    std::string expectedStringForFirstTwentySymbols;
    std::string expectedStringForLastTwentySymbols;

    for(std::size_t i = 0; i < 20; ++i) {
        char currSymbol = static_cast<char>('a' + i);
        lowerCaseAlphabetFirstSymbols[i] = static_cast<std::byte>(currSymbol);
        expectedStringForFirstTwentySymbols.push_back(currSymbol);
    }

    std::string result;
    EXPECT_NO_THROW(result = hash::urlEncodeHash(lowerCaseAlphabetFirstSymbols));
    EXPECT_EQ(result, expectedStringForFirstTwentySymbols);

    const std::size_t diff = alphabetSize - lowerCaseAlphabetLastSymbols.size();
    for(std::size_t i = diff; i < alphabetSize; ++i) {
        char currSymbol = static_cast<char>('a' + i);
        lowerCaseAlphabetLastSymbols[i - diff] = static_cast<std::byte>(currSymbol);
        expectedStringForLastTwentySymbols.push_back(currSymbol);
    }
    EXPECT_NO_THROW(result = hash::urlEncodeHash(lowerCaseAlphabetLastSymbols));
    EXPECT_EQ(result, expectedStringForLastTwentySymbols);
}

TEST(HashMethodsTest, TestUrlEncodeWithUpperCaseAlphabet) {
    const std::size_t alphabetSize = 26;
    std::array<std::byte, 20> upperCaseAlphabetFirstSymbols{};
    std::array<std::byte, 20> upperCaseAlphabetLastSymbols{};
    std::string expectedStringForFirstTwentySymbols;
    std::string expectedStringForLastTwentySymbols;

    for(std::size_t i = 0; i < 20; ++i) {
        char currSymbol = static_cast<char>('A' + i);
        upperCaseAlphabetFirstSymbols[i] = static_cast<std::byte>(currSymbol);
        expectedStringForFirstTwentySymbols.push_back(static_cast<char>(currSymbol));
    }

    std::string result;
    EXPECT_NO_THROW(result = hash::urlEncodeHash(upperCaseAlphabetFirstSymbols));
    EXPECT_EQ(result, expectedStringForFirstTwentySymbols);

    const std::size_t diff = alphabetSize - upperCaseAlphabetLastSymbols.size();
    for(std::size_t i = diff; i < alphabetSize; ++i) {
        char currSymbol = static_cast<char>('A' + i);
        upperCaseAlphabetLastSymbols[i - diff] = static_cast<std::byte>(currSymbol);
        expectedStringForLastTwentySymbols.push_back(currSymbol);
    }
    EXPECT_NO_THROW(result = hash::urlEncodeHash(upperCaseAlphabetLastSymbols));
    EXPECT_EQ(result, expectedStringForLastTwentySymbols);
}

TEST(HashMethodsTest, TestUrlEncodeWithDigits) {
    std::array<std::byte, 20> digits{};
    std::string expectedString;

    for(std::size_t i = 0; i < 20; ++i) {
        char currSymbol = static_cast<char>('0' + (i % 9));
        digits[i] = static_cast<std::byte>(currSymbol);
        expectedString.push_back(currSymbol);
    }

    std::string result;
    EXPECT_NO_THROW(result = hash::urlEncodeHash(digits));
    EXPECT_EQ(result, expectedString);
}

TEST(HashMethodsTest, TestUrlEncodeWithSpaceSymbols) {
    std::array<std::byte, 20> hash{};
    std::string expectedString;

    const char spaceSymbol = ' ';

    for(std::size_t i = 0; i < 20; ++i) {
        hash[i] = static_cast<std::byte>(spaceSymbol);
        expectedString.push_back('+');
    }

    std::string result;
    EXPECT_NO_THROW(result = hash::urlEncodeHash(hash));
    EXPECT_EQ(result, expectedString);
}

TEST(HashMethodsTest, TestUrlEncodeWithNonAlphaAndNonDigitSymbols) {
    const std::size_t hashSize = 20;
    std::array<std::byte, hashSize> hash{};
    std::vector<std::string> expectedSymbols(20, "%00");

    std::size_t counter = 0;
    for(int i = 0; i < 255; ++i) {
        auto currByte = static_cast<std::byte>(i);
        auto currSymbol = static_cast<char>(currByte);
        if(std::isalpha(currSymbol) || std::isdigit(currSymbol) || currSymbol == ' ')
            continue;
        hash[counter % hashSize] = static_cast<std::byte>(currSymbol);
        std::stringstream ss;
        ss << std::hex << std::uppercase << static_cast<int>(currByte);
        if(ss.str().size() == 1) {
            expectedSymbols[counter % hashSize] = std::string("%") + "0" + ss.str();
        } else {
            expectedSymbols[counter % hashSize] = std::string("%") + ss.str();
        }
        ++counter;

        std::string expectedString = std::accumulate(expectedSymbols.begin(), expectedSymbols.end(), std::string(""));
        std::string result;
        EXPECT_NO_THROW(result = hash::urlEncodeHash(hash));
        EXPECT_EQ(result.size(), 60);
        EXPECT_EQ(expectedString, result);
    }
}
