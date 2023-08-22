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
    const std::vector<std::byte> expectedResult = {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x1),
                                                   std::byte(0x0)};

    const std::vector<std::byte> result = builder.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST_F(MessageBuilderFixture, SetPayloadInBuilderAndBuildMessageTypeWitouthPayloadPart) {
    const MessageType messageTypeWithoutPayload = MessageType::CHOKE;
    const std::vector<std::byte> payload{std::byte(0xFF), std::byte(0xAA)};
    const std::vector<std::byte> expectedResult = {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x1),
                                                   std::byte(0x0)};
    builder.setMessageType(messageTypeWithoutPayload);
    builder.setPayload(payload);

    const std::vector<std::byte> result = builder.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST_F(MessageBuilderFixture, BuildMessageWithPayload) {
    const MessageType messageTypeWithPayload = MessageType::HAVE;
    const std::vector<std::byte> payload{std::byte(0xFF), std::byte(0xAA)};
    const std::vector<std::byte> expectedResult = {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x3),
                                                   std::byte(0x4),
                                                   payload[0], payload[1]};
    builder.setMessageType(messageTypeWithPayload);
    builder.setPayload(payload);

    const std::vector<std::byte> result = builder.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST_F(MessageBuilderFixture, BuildKeepAliveMessage) {
    const MessageType messageTypeKeepAlive = MessageType::KEEP_ALIVE;
    const std::vector<std::byte> expectedResult = {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x0)};
    builder.setMessageType(messageTypeKeepAlive);

    const std::vector<std::byte> result = builder.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST_F(MessageBuilderFixture, BuildKeepAliveMessageWithPayload) {
    const MessageType messageTypeKeepAlive = MessageType::KEEP_ALIVE;
    const std::vector<std::byte> payload {std::byte(0x1), std::byte(0x2), std::byte(0x3), std::byte(0x4)};
    const std::vector<std::byte> expectedResult {
                                                    std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x0),
                                                };
    builder.setMessageType(messageTypeKeepAlive);
    builder.setPayload(payload);

    const std::vector<std::byte> result = builder.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(ConverterArgsToPayloadStringTest, BuildPayloadStringForHaveMessageType) {
    const MessageType messageType = MessageType::HAVE;
    const int pieceLength = 123;
    const std::vector<std::byte> expectedResult = {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(123)};

    const std::vector<std::byte> result = ConverterArgsToPayloadString<messageType>::getPayload(pieceLength);

    EXPECT_EQ(expectedResult, result);
}

TEST(ConverterArgsToPayloadStringTest, BuildEmptyBitfieldMessage) {
    const MessageType messageType = MessageType::BITFIELD;
    const std::vector<std::byte> bitfield = {};

    const std::vector<std::byte> result = ConverterArgsToPayloadString<messageType>::getPayload(bitfield);

    EXPECT_TRUE(result.empty());
}

TEST(ConverterArgsToPayloadStringTest, BuildBitfieldMessageWithOneBytes) {
    const MessageType messageType = MessageType::BITFIELD;
    const std::vector<std::byte> bitfield = {std::byte(0x1)};
    const std::vector<std::byte> expectedResult {std::byte(0x1)};

    const std::vector<std::byte> result = ConverterArgsToPayloadString<messageType>::getPayload(bitfield);

    EXPECT_EQ(expectedResult, result);
}

TEST(ConverterArgsToPayloadStringTest, BuildPayloadStringForBitfieldMessageType) {
    const MessageType messageType = MessageType::BITFIELD;
    const std::vector<std::byte> bitfield = {std::byte(0x1), std::byte(0x2), std::byte(0x3)};
    const std::vector<std::byte> expectedResult {bitfield[0], bitfield[1], bitfield[2]};

    const std::vector<std::byte> result = ConverterArgsToPayloadString<messageType>::getPayload(bitfield);

    EXPECT_EQ(expectedResult, result);
}

TEST(ConverterArgsToPayloadStringTest, BuildPayloadStringForRequestMessageType) {
    const MessageType messageType = MessageType::REQUEST;
    const int index = 1;
    const int begin = 2;
    const int length = 3;
    const std::vector<std::byte> expectedResult {
                                                    std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(index),
                                                    std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(begin),
                                                    std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(length),
                                                };

    const std::vector<std::byte> result = ConverterArgsToPayloadString<messageType>::getPayload(index, begin, length);

    EXPECT_EQ(expectedResult, result);
}

TEST(ConverterArgsToPayloadStringTest, BuildPayloadStringForPieceMessageType) {
    const MessageType messageType = MessageType::PIECE;
    const int index = 1;
    const int begin = 2;
    const std::vector<std::byte> block {std::byte(0xAA), std::byte(0xBB), std::byte(0xCC)};
    const std::vector<std::byte> expectedResult {
                                                    std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(index),
                                                    std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(begin),
                                                    block[0], block[1], block[2]
                                                };

    const std::vector<std::byte> result = ConverterArgsToPayloadString<messageType>::getPayload(index, begin, block);

    EXPECT_EQ(expectedResult, result);
}

TEST(ConverterArgsToPayloadStringTest, BuildPayloadStringForCancelMessageType) {
    const MessageType messageType = MessageType::CANCEL;
    const int index = 1;
    const int begin = 2;
    const int length = 3;
    const std::vector<std::byte> expectedResult {
                                                      std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(index),
                                                      std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(begin),
                                                      std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(length),
                                                };

    const std::vector<std::byte> result = ConverterArgsToPayloadString<messageType>::getPayload(index, begin, length);

    EXPECT_EQ(expectedResult, result);
}

TEST(KeepAliveClassTest, GetMessageFromKeepAliveClass) {
    const std::vector<std::byte> expectedResult {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x0)};

    const std::vector<std::byte> result = KeepAlive().getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(ChokeClassTest, GetMessageFromChokeClass) {
    const std::vector<std::byte> expectedResult {
                                                    std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x1),
                                                    std::byte(0x0)
                                                };
    Choke choke;

    const std::vector<std::byte> result = choke.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(UnchokeClassTest, GetMessageFromUnchokeClass) {
    const std::vector<std::byte> expectedResult {
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x1),
                                                     std::byte(0x1),
                                                };
    Unchoke unchoke;

    const std::vector<std::byte> result = unchoke.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(InterestedClassTest, GetMessageFromInterestedClass) {
    const std::vector<std::byte> expectedResult {
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x1),
                                                     std::byte(0x2),
                                                };
    Interested interested;

    const std::vector<std::byte> result = interested.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(NotInterestedClassTest, GetMessageFromNotInterestedClass) {
    const std::vector<std::byte> expectedResult {
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x1),
                                                     std::byte(0x3),
                                                };
    NotInterested notInterested;

    const std::vector<std::byte> result = notInterested.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(HaveClassTest, GetMessageFromHaveClass) {
    const int pieceIndex = 12;
    const std::vector<std::byte> expectedResult {
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x5),
                                                     std::byte(0x4),
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(12),
                                                };
    Have have(pieceIndex);

    const std::vector<std::byte> result = have.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(BitfieldClassTest, GetMessageFromBitfieldClass) {
    const std::vector<std::byte> bitfieldValue {std::byte(0x1), std::byte(0x2), std::byte(0x3)};
    const std::vector<std::byte> expectedResult {
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x4),
                                                     std::byte(0x5),
                                                     bitfieldValue[0], bitfieldValue[1], bitfieldValue[2],
                                                };
    Bitfield bitfield(bitfieldValue);

    const std::vector<std::byte> result = bitfield.getMessage();
}

TEST(RequestClassTest, GetMessageFromRequestClass) {
    const int index = 1;
    const int begin = 2;
    const int length = 3;
    Request request(index, begin, length);
    const std::vector<std::byte> expectedResult {
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(13),
                                                     std::byte(0x6),
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(index),
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(begin),
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(length),
                                                };

    const std::vector<std::byte> result = request.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(PieceClassTest, GetMessageFromPieceClass) {
    const int index = 1;
    const int begin = 2;
    const std::vector<std::byte> block {std::byte(0xAA), std::byte(0xBB), std::byte(0xCC)};
    Piece piece(index, begin, block);

    const std::vector<std::byte> expectedResult {
                                                    std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(12),
                                                    std::byte(0x7),
                                                    std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(index),
                                                    std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(begin),
                                                    block[0], block[1], block[2],
                                                };

    const std::vector<std::byte> result = piece.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(CancelClassTest, GetMessageFromCancelClass) {
    const int index = 1;
    const int begin = 2;
    const int length = 3;
    Cancel cancel(index, begin, length);
    const std::vector<std::byte> expectedResult {
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(13),
                                                     std::byte(0x8),
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(index),
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(begin),
                                                     std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(length),
                                                };

    const std::vector<std::byte> result = cancel.getMessage();

    EXPECT_EQ(expectedResult, result);
}

TEST(PortClasstest, GetMessageFromPortClass) {
    const unsigned short portValue = 16706;
    Port port(portValue);
    const std::vector<std::byte> expectedResult {
                                                    std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x3),
                                                    std::byte(0x9),
                                                    std::byte(65), std::byte(66)
                                                };

    std::vector<std::byte> result = port.getMessage();

    EXPECT_EQ(expectedResult, result);
}