#pragma once

#include "Bittorrent/BittorrentMessages/BittorrentMessageParser.hpp"

#include <gtest/gtest.h>

TEST(BittorrentMessageParserTest, TestParseEmptyMessage) {
    const std::vector<std::byte> rawMessage;

    const std::vector<BittorrentMessage> result = BittorrentMessageParser::getMessages(rawMessage);

    EXPECT_TRUE(result.empty());
}

TEST(BittorrentMessageParser, TestParseKeepAliveMessage) {
    const std::vector<std::byte> rawMessage {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x0)};

    const std::vector<BittorrentMessage> result = BittorrentMessageParser::getMessages(rawMessage);

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].getMessageType(), MessageType::KEEP_ALIVE);
    EXPECT_TRUE(result[0].getPayload().empty());
}

TEST(BittorrentMessageParser, TestParseChokeMessage) {
    const MessageType messageType = MessageType::CHOKE;
    const std::vector<std::byte> rawMessage {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x1),
                                             static_cast<std::byte>(messageType)};

    const std::vector<BittorrentMessage> result = BittorrentMessageParser::getMessages(rawMessage);

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].getMessageType(), MessageType::CHOKE);
    EXPECT_TRUE(result[0].getPayload().empty());
}

TEST(BittorrentMessageParser, TestParseMessageWithPayload) {
    const MessageType messageType = MessageType::HAVE;
    const std::vector<std::byte> rawMessage {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x5),
                                             static_cast<std::byte>(messageType),
                                             std::byte(0x1), std::byte(0x2), std::byte(0x3), std::byte(0x4)};

    const std::vector<BittorrentMessage> result = BittorrentMessageParser::getMessages(rawMessage);

    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].getMessageType(), MessageType::HAVE);
    EXPECT_EQ(result[0].getPayload().size(), 4);

    const auto payload = result[0].getPayload();

    EXPECT_EQ(payload[0], std::byte(0x1));
    EXPECT_EQ(payload[1], std::byte(0x2));
    EXPECT_EQ(payload[2], std::byte(0x3));
    EXPECT_EQ(payload[3], std::byte(0x4));
}

TEST(BittorrentMessageParser, TestParseFewMessagesWitouthPayload) {
    const MessageType firstMessageType = MessageType::CHOKE;
    const MessageType secondMessageType = MessageType::INTERESTED;
    const std::vector<std::byte> firstRawMessage {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x1),
                                                  static_cast<std::byte>(firstMessageType)};
    const std::vector<std::byte> secondRawMessage {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x1),
                                                  static_cast<std::byte>(secondMessageType)};

    std::vector<std::byte> rawMessage;
    rawMessage.insert(rawMessage.end(), firstRawMessage.begin(), firstRawMessage.end());
    rawMessage.insert(rawMessage.end(), secondRawMessage.begin(), secondRawMessage.end());

    const std::vector<BittorrentMessage> result = BittorrentMessageParser::getMessages(rawMessage);

    EXPECT_EQ(result.size(), 2);

    const BittorrentMessage firstMessage = result[0];

    EXPECT_EQ(firstMessage.getMessageType(), firstMessageType);
    EXPECT_TRUE(firstMessage.getPayload().empty());

    const BittorrentMessage secondMessage = result[1];

    EXPECT_EQ(secondMessage.getMessageType(), secondMessageType);
    EXPECT_TRUE(secondMessage.getPayload().empty());
}

TEST(BittorrentMessageParser, TestParseFewMessagesWithPayload) {
    const MessageType firstMessageType = MessageType::HAVE;
    const MessageType secondMessageType = MessageType::BITFIELD;
    const std::vector<std::byte> firstRawMessage {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x5),
                                                  static_cast<std::byte>(firstMessageType),
                                                  std::byte(0x1), std::byte(0x2), std::byte(0x3), std::byte(0x4)};
    const std::vector<std::byte> secondRawMessage {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x2),
                                                   static_cast<std::byte>(secondMessageType),
                                                   std::byte(0x5)};

    std::vector<std::byte> rawMessage;
    rawMessage.insert(rawMessage.end(), firstRawMessage.begin(), firstRawMessage.end());
    rawMessage.insert(rawMessage.end(), secondRawMessage.begin(), secondRawMessage.end());

    const std::vector<BittorrentMessage> result = BittorrentMessageParser::getMessages(rawMessage);

    EXPECT_EQ(result.size(), 2);

    const BittorrentMessage firstMessage = result[0];

    EXPECT_EQ(firstMessage.getMessageType(), firstMessageType);
    EXPECT_EQ(firstMessage.getPayload().size(), 4);

    const auto firstMessagePayload = firstMessage.getPayload();

    EXPECT_EQ(firstMessagePayload[0], std::byte(0x1));
    EXPECT_EQ(firstMessagePayload[1], std::byte(0x2));
    EXPECT_EQ(firstMessagePayload[2], std::byte(0x3));
    EXPECT_EQ(firstMessagePayload[3], std::byte(0x4));

    const BittorrentMessage secondMessage = result[1];

    EXPECT_EQ(secondMessage.getMessageType(), secondMessageType);
    EXPECT_EQ(secondMessage.getPayload().size(), 1);

    EXPECT_EQ(secondMessage.getPayload()[0], std::byte(0x5));
}

//TODO add complex test for messages with payload and without payload in different subsequence
