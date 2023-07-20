//
// Created by ctuh on 7/20/23.
//

#pragma once

#include <gtest/gtest.h>
#include "BencodeParser.hpp"
#include <sstream>


TEST(BencodeParserTest, testParseSimpleBencodeString) {
    const std::string fieldName = "announce";
    const std::string fieldValue = "someFieldValue";
    std::stringstream ss;
    ss << "d" << fieldName.size() << ":" << fieldName << fieldValue.size() << ":" << fieldValue;

    const std::string bencodeString = ss.str();

    BencodeParser parser(bencodeString);

    std::string result;

    ASSERT_NO_THROW(result = parser["announce"]);
    ASSERT_EQ(result, fieldValue);
}

TEST(BencodeParserTest, testParseSimpleBencodeStringTwice) {
    const std::string fieldName = "announce";
    const std::string fieldValue = "someFieldValue";
    std::stringstream ss;
    ss << "d" << fieldName.size() << ":" << fieldName << fieldValue.size() << ":" << fieldValue;

    const std::string bencodeString = ss.str();

    BencodeParser parser(bencodeString);

    std::string result;

    ASSERT_NO_THROW(result = parser["announce"]);
    ASSERT_EQ(result, fieldValue);
    ASSERT_NO_THROW(result = parser["announce"]);
    ASSERT_EQ(result, fieldValue);
}

TEST(BencodeParserTest, testParseList) {
    const std::string fieldName = "prettyList";
    const std::string fieldValue = "IAmListElement";
    const int listSize = 5; // for simple logic test should be not greater than 9

    std::stringstream ss;

    ss << "d" << fieldName.size() << ':' << fieldName << 'l' << fieldValue.size() + 1 << ':';

    for(int i = 1; i < listSize; ++i) {
        ss << fieldValue << i << 'e' << 'l' << fieldValue.size() + 1 << ':';
    }
    ss << fieldValue << listSize << 'e' << 'e';


    const std::string bencodeString = ss.str();

    BencodeParser parser(bencodeString);

    StringVector result;

    ASSERT_NO_THROW(result = parser.getList(fieldName));
    ASSERT_TRUE(result.size() == listSize);

    for(int i = 1; i <= listSize; ++i) {
        ASSERT_EQ(result[i-1], fieldValue + std::to_string(i));
    }
}

TEST(BencodeParserTest, testParseListTwice) {
    const std::string fieldName = "prettyList";
    const std::string fieldValue = "IAmListElement";
    const int listSize = 5; // for simple logic test should be not greater than 9

    std::stringstream ss;

    ss << "d" << fieldName.size() << ':' << fieldName << 'l' << fieldValue.size() + 1 << ':';

    for(int i = 1; i < listSize; ++i) {
        ss << fieldValue << i << 'e' << 'l' << fieldValue.size() + 1 << ':';
    }
    ss << fieldValue << listSize << 'e' << 'e';


    const std::string bencodeString = ss.str();

    BencodeParser parser(bencodeString);

    StringVector result;

    ASSERT_NO_THROW(result = parser.getList(fieldName));
    ASSERT_TRUE(result.size() == listSize);

    for(int i = 1; i <= listSize; ++i) {
        ASSERT_EQ(result[i-1], fieldValue + std::to_string(i));
    }

    ASSERT_NO_THROW(result = parser.getList(fieldName));
    ASSERT_TRUE(result.size() == listSize);

    for(int i = 1; i <= listSize; ++i) {
        ASSERT_EQ(result[i-1], fieldValue + std::to_string(i));
    }
}