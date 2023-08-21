//
// Created by ctuh on 8/12/23.
//

#pragma once

#include <gtest/gtest.h>
#include "BittorrentMessages/BittorrentMessageType.hpp"
#include <unordered_set>
#include "BittorrentMessages/BittorrentMessageBuilder.hpp"
#include "BittorrentMessages/ConverterArgsToPayloadString.hpp"

#include "BittorrentMessages/KeepAlive.hpp" // TODO may be create header file only with this includes?
#include "BittorrentMessages/Choke.hpp"
#include "BittorrentMessages/Unchoke.hpp"
#include "BittorrentMessages/Interested.hpp"
#include "BittorrentMessages/NotInterested.hpp"
#include "BittorrentMessages/Have.hpp"
#include "BittorrentMessages/Bitfield.hpp"
#include "BittorrentMessages/Request.hpp"
#include "BittorrentMessages/Piece.hpp"
#include "BittorrentMessages/Cancel.hpp"
#include "BittorrentMessages/Port.hpp"

class MessageBuilderFixture: public ::testing::Test {
public:
    void SetUp() override {}
    void TearDown() override {};

public:
    BittorrentMessageBuilder builder;
};

TEST_F(MessageBuilderFixture, BuildMessageWitoutPayload) {
    const MessageType messageTypeWithoutPayload = MessageType::CHOKE;
    builder.setMessageType(messageTypeWithoutPayload);
    const std::string expectedResult = "10";

    const std::string result = builder.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST_F(MessageBuilderFixture, SetPayloadInBuilderAndBuildMessageTypeWitouthPayloadPart) {
    const MessageType messageTypeWithoutPayload = MessageType::CHOKE;
    builder.setMessageType(messageTypeWithoutPayload);
    builder.setPayload("somePayload");
    const std::string expectedResult = "10";

    const std::string result = builder.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST_F(MessageBuilderFixture, BuildMessageWithPayload) {
    const MessageType messageTypeWithPayload = MessageType::HAVE;
    builder.setMessageType(messageTypeWithPayload);
    builder.setPayload("1234");
    const std::string expectedResult = "541234";

    const std::string result = builder.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST_F(MessageBuilderFixture, BuildKeepAliveMessage) {
    const MessageType messageTypeKeepAlive = MessageType::KEEP_ALIVE;
    builder.setMessageType(messageTypeKeepAlive);
    const std::string expectedResult = "0";

    const std::string result = builder.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST_F(MessageBuilderFixture, BuildKeepAliveMessageWithPayload) {
    const MessageType messageTypeKeepAlive = MessageType::KEEP_ALIVE;
    builder.setMessageType(messageTypeKeepAlive);
    builder.setPayload("1234");
    const std::string expectedResult = "0";

    const std::string result = builder.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(ConverterArgsToPayloadStringTest, BuildPayloadStringForHaveMessageType) {
    const MessageType messageType = MessageType::HAVE;
    const int pieceLength = 123;
    const std::string expectedResult = "0123";

    const std::string result = ConverterArgsToPayloadString<messageType>::getPayload(pieceLength);

    EXPECT_EQ(expectedResult, result);
}

TEST(ConverterArgsToPayloadStringTest, BuildEmptyBitfieldMessage) {
    const MessageType messageType = MessageType::BITFIELD;
    const std::vector<std::byte> bitfield = {};

    const std::string result = ConverterArgsToPayloadString<messageType>::getPayload(bitfield);

    EXPECT_TRUE(result.empty());
}

TEST(ConverterArgsToPayloadStringTest, BuildBitfieldMessageWithOneBytes) {
    const MessageType messageType = MessageType::BITFIELD;
    const std::vector<std::byte> bitfield = {std::byte(0x1)};

    const std::string expectedResult(1, static_cast<char>(std::byte(0x1)));

    const std::string result = ConverterArgsToPayloadString<messageType>::getPayload(bitfield);

    EXPECT_EQ(expectedResult, result);
}

TEST(ConverterArgsToPayloadStringTest, BuildPayloadStringForBitfieldMessageType) {
    const MessageType messageType = MessageType::BITFIELD;
    const std::vector<std::byte> bitfield = {std::byte(0x1), std::byte(0x2), std::byte(0x3)};
    const std::string expectedResult = std::string("") + static_cast<char>(0x1) + static_cast<char>(0x2) + static_cast<char>(0x3);

    const std::string result = ConverterArgsToPayloadString<messageType>::getPayload(bitfield);

    EXPECT_EQ(expectedResult, result);
}

TEST(ConverterArgsToPayloadStringTest, BuildPayloadStringForRequestMessageType) {
    const MessageType messageType = MessageType::REQUEST;
    const int index = 1;
    const int begin = 2;
    const int length = 3;
    const std::string expectedResult = "000100020003";

    const std::string result = ConverterArgsToPayloadString<messageType>::getPayload(index, begin, length);

    EXPECT_EQ(expectedResult, result);
}

TEST(ConverterArgsToPayloadStringTest, BuildPayloadStringForPieceMessageType) {
    const MessageType messageType = MessageType::PIECE;
    const int index = 1;
    const int begin = 2;
    const std::vector<char> block {'a', 'b', 'c'};
    const std::string expectedResult = "00010002abc";

    const std::string result = ConverterArgsToPayloadString<messageType>::getPayload(index, begin, block);

    EXPECT_EQ(expectedResult, result);
}

TEST(KeepAliveClassTest, GetMessageFromKeepAliveClass) {
    const std::string expectedResult = "0";

    const std::string result = KeepAlive().getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(ChokeClassTest, GetMessageFromChokeClass) {
    const std::string expectedResult = "10";
    Choke choke;

    const std::string result = choke.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(UnchokeClassTest, GetMessageFromUnchokeClass) {
    const std::string expectedResult = "11";
    Unchoke unchoke;

    const std::string result = unchoke.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(InterestedClassTest, GetMessageFromInterestedClass) {
    const std::string expectedResult = "12";
    Interested interested;

    const std::string result = interested.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(NotInterestedClassTest, GetMessageFromNotInterestedClass) {
    const std::string expectedResult = "13";
    NotInterested notInterested;

    const std::string result = notInterested.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(HaveClassTest, GetMessageFromHaveClass) {
    const int pieceIndex = 12;
    const std::string expectedResult = "540012";
    Have have(pieceIndex);

    const std::string result = have.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(BitfieldClassTest, GetMessageFromBitfieldClass) {
    const std::vector<std::byte> bitfieldValue {std::byte(0x1), std::byte(0x2), std::byte(0x3)};
    const std::string expectedResult = std::string("45") + static_cast<char>(std::byte(0x1))
                                                         + static_cast<char>(std::byte(0x2))
                                                         + static_cast<char>(std::byte(0x3));
    Bitfield bitfield(bitfieldValue);

    const std::string result = bitfield.getMessage();
}

TEST(RequestClassTest, GetMessageFromRequestClass) {
    const int index = 1;
    const int begin = 2;
    const int length = 3;
    Request request(index, begin, length);
    const std::string expectedResult = "136000100020003";

    const std::string result = request.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(PieceClassTest, GetMessageFromPieceClass) {
    const int index = 1;
    const int begin = 2;
    const std::vector<char> block {'a', 'b', 'c'};
    Piece piece(index, begin, block);
    const std::string expectedResult = "12700010002abc";

    const std::string result = piece.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(CancelClassTest, GetMessageFromCancelClass) {
    const int index = 1;
    const int begin = 2;
    const int length = 3;
    Cancel cancel(index, begin, length);
    const std::string expectedResult = "138000100020003";

    const std::string result = cancel.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(PortClasstest, GetMessageFromPortClass) {
    const unsigned short portValue = 16706;
    Port port(portValue);
    const std::string expectedResult = "39AB";

    std::string result = port.getMessage();

    EXPECT_EQ(expectedResult, result);
}