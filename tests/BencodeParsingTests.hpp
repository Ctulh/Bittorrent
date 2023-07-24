//
// Created by ctuh on 7/20/23.
//

#pragma once

#include <gtest/gtest.h>
#include "BencodeParser.hpp"
#include "Exceptions/NoSuchFieldException.hpp"
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

TEST(BencodeParserTest, testParseDictionaryInAnotherDictionary) {
    const std::string topLevelFieldName = "topLevelDictionaryFieldName";
    const std::string fieldName= "someFieldName";
    const std::string fieldValue = "someFieldValue";

    std::stringstream ss;
    std::stringstream topLevelDictionaryValue;

    topLevelDictionaryValue << fieldName << 'd' << fieldValue.size() << ':' << fieldValue << 'e';
    ss << "d" << topLevelFieldName.size() <<  ':' << topLevelFieldName << 'd' << fieldName.size() << ':' << topLevelDictionaryValue.str() << 'e';

    const std::string bencodeString = ss.str();

    BencodeParser parser(bencodeString);


    std::string result;
    ASSERT_NO_THROW(result = parser.getValue(topLevelFieldName));
    ASSERT_EQ(result, topLevelDictionaryValue.str());
}

TEST(BencodeParserTest, testParseListOfDictionaries) {
    const std::string topLevelFieldName = "info";
    const std::string listFieldName = "files";
    const std::string fieldName = "length";

    std::stringstream firstListElement;
    firstListElement << 'd' << fieldName.size() << ':' << fieldName << 'i' << 1 << 'e' << 'e';
    std::stringstream secondListElement;
    firstListElement << 'd' << fieldName.size() << ':' << fieldName << 'i' << 2 << 'e' << 'e';
    std::stringstream ss;

    ss << 'd' << topLevelFieldName.size() << ':' << topLevelFieldName << 'd' << listFieldName.size() << ':' << listFieldName << 'l' << 'd' << fieldName.size() << ':' << fieldName << 'i' << 1 << 'e' << 'e' << 'd' << fieldName.size() << ':' << fieldName << 'i' << 2 << 'e' << 'e' << 'e';

    const std::string bencodeString = ss.str();

    BencodeParser parser(bencodeString);

    StringVector result;
    ASSERT_NO_THROW(result = parser.getList(listFieldName));
    ASSERT_TRUE(result.size() == 2);
}

TEST(BencodeParserTest, testParseList) {
    const std::string fieldName = "prettyList";
    const std::string fieldValue = "IAmListElement";
    const int listSize = 5; // for simple logic test should be not greater than 9

    std::stringstream ss;

    ss << "d" << fieldName.size() << ':' << fieldName << 'l' << fieldValue.size() + 1 << ':';

    for(int i = 1; i < listSize; ++i) {
        ss << fieldValue << i << fieldValue.size() + 1 << ':';
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

TEST(BencodeParserTest, testParseDictionaryAsList) {
    const std::string fieldName = "someFieldName";
    const std::string fieldValue = "someFieldValue";

    std::stringstream ss;
    ss << 'd' << fieldName.size() << ':' << fieldName << fieldValue.size() << ':' << fieldValue << 'e';

    const std::string bencodeString = ss.str();

    BencodeParser parser(bencodeString);

    StringVector result;

    ASSERT_NO_THROW(result = parser.getList(fieldName));
    ASSERT_TRUE(result.size() == 1);
    ASSERT_EQ(result[0], fieldValue);
}

TEST(BencodeParserTest, testGetUnexistFieldValue) {
    const std::string fieldName = "someFieldName";
    const std::string fieldValue = "someFieldValue";

    std::stringstream ss;
    ss << 'd' << fieldName.size() << ':' << fieldName << fieldValue.size() << ':' << fieldValue << 'e';

    const std::string bencodeString = ss.str();

    BencodeParser parser(bencodeString);

    const std::string wrongFieldName = "wrongFieldName";

    ASSERT_THROW(parser.getValue(wrongFieldName), NoSuchFieldException);
}

TEST(BencodeParserTest, testGetUnexistFieldValueTwice) {
    const std::string fieldName = "someFieldName";
    const std::string fieldValue = "someFieldValue";

    std::stringstream ss;
    ss << 'd' << fieldName.size() << ':' << fieldName << fieldValue.size() << ':' << fieldValue << 'e';

    const std::string bencodeString = ss.str();

    BencodeParser parser(bencodeString);

    const std::string wrongFieldName = "wrongFieldName";

    ASSERT_THROW(parser.getValue(wrongFieldName), NoSuchFieldException);
    ASSERT_THROW(parser.getValue(wrongFieldName), NoSuchFieldException);
}

TEST(BencodeParserTest, testGetUnexistList) {
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

    const std::string wrongFieldName = "wrongFieldName";

    ASSERT_THROW(parser.getList(wrongFieldName), NoSuchFieldException);
}

TEST(BencodeParserTest, testGetUnexistListTwice) {
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

    const std::string wrongFieldName = "wrongFieldName";

    ASSERT_THROW(parser.getList(wrongFieldName), NoSuchFieldException);
    ASSERT_THROW(parser.getList(wrongFieldName), NoSuchFieldException);
}

TEST(BencodeParserTest, testParseIntValue) {
    const std::string fieldName = "SomeId";
    const int id = 6;

    std::stringstream ss;
    ss << 'd' << fieldName.size() << ':' << fieldName << 'i' << id << 'e' << 'e';

    const std::string bencodeString = ss.str();

    BencodeParser parser(bencodeString);

    std::string result;

    ASSERT_NO_THROW(result = parser.getValue(fieldName));
    ASSERT_EQ(result, std::to_string(id));
}