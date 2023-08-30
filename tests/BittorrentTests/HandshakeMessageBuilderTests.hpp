#pragma once

#include "Bittorrent/BittorrentMessages/HandshakeMessageBuilder.hpp"

#include <gtest/gtest.h>

TEST(HandshakeMessageBuilderTest, contructHanshakeBuilder) {
    EXPECT_NO_THROW(HandshakeMessageBuilder());
}

TEST(HandshakeMessageBuilderTest, getMessageByDefault) {
    const std::size_t expectedMessageSize = 68;
    const std::string pstr = "BitTorrent protocol";
    std::string pstrLen;
    pstrLen.push_back(pstr.size());
    const std::string firstTwentyBytes = pstrLen + pstr;
    HandshakeMessageBuilder messageBuilder;

    const std::string handshake = messageBuilder.getMessage();

    EXPECT_EQ(handshake.size(), expectedMessageSize);
    EXPECT_EQ(handshake.substr(0, 20), firstTwentyBytes);
}

TEST(HandshakeMessageBuilderTest, setReserved) {
    const std::size_t expectedMessageSize = 68;
    const std::string reserved = "12345678";
    HandshakeMessageBuilder messageBuilder;

    messageBuilder.setReserved(reserved);
    const std::string handshake = messageBuilder.getMessage();

    EXPECT_EQ(handshake.size(), expectedMessageSize);
    EXPECT_EQ(handshake.substr(20, 8), reserved);
}

TEST(HandshakeMessageBuilderTest, setReservedLongerThanEightBytes) {
    const std::size_t expectedMessageSize = 68;
    const std::string expectedReserved = "12345678";
    const std::string reserved = expectedReserved + "9";
    HandshakeMessageBuilder messageBuilder;

    messageBuilder.setReserved(reserved);
    const std::string handshake = messageBuilder.getMessage();

    EXPECT_EQ(handshake.size(), expectedMessageSize);
    EXPECT_EQ(handshake.substr(20, 8), expectedReserved);
}

TEST(HandshakeMessageBuilderTest, setReservedShorterThanEightBytes) {
    const std::size_t expectedMessageSize = 68;
    const std::string reserved = "1234567";
    std::string expectedReserved = reserved;
    expectedReserved.push_back(0x0);
    HandshakeMessageBuilder messageBuilder;

    messageBuilder.setReserved(reserved);
    const std::string handshake = messageBuilder.getMessage();

    EXPECT_EQ(handshake.size(), expectedMessageSize);
    EXPECT_EQ(handshake.substr(20, 8), expectedReserved);
}

TEST(HandshakeMessageBuilderTest, setInfoHash) {
    const std::size_t expectedMessageSize = 68;
    HandshakeMessageBuilder messageBuilder;
    const std::string infoHash = "justTwentyByteString";

    messageBuilder.setInfoHash(infoHash);

    const std::string handshake = messageBuilder.getMessage();

    EXPECT_EQ(handshake.size(), expectedMessageSize);
    EXPECT_EQ(handshake.substr(28, 20), infoHash);
}

TEST(HandshakeMessageBuilderTest, setInfoHashLongerThanTwentyBytes) {
    const std::size_t expectedMessageSize = 68;
    HandshakeMessageBuilder messageBuilder;
    const std::string expectedInfoHash = "justTwentyByteString";
    const std::string infoHash = expectedInfoHash + "a";

    messageBuilder.setInfoHash(infoHash);

    const std::string handshake = messageBuilder.getMessage();

    EXPECT_EQ(handshake.size(), expectedMessageSize);
    EXPECT_EQ(handshake.substr(28, 20), expectedInfoHash);
}

TEST(HandshakeMessageBuilderTest, setInfoHashShorterThanTwentyBytes) {
    const std::size_t expectedMessageSize = 68;
    HandshakeMessageBuilder messageBuilder;
    std::string expectedInfoHash;
    for(int i = 0; i < 20; ++i) {
        expectedInfoHash.push_back(0x0);
    }
    const std::string infoHash = "infoHash";

    messageBuilder.setInfoHash(infoHash);

    const std::string handshake = messageBuilder.getMessage();

    EXPECT_EQ(handshake.size(), expectedMessageSize);
    EXPECT_EQ(handshake.substr(28,20), expectedInfoHash);
}

TEST(HandshakeMessageBuilder, setPeerId) {
    const std::size_t expectedMessageSize = 68;
    HandshakeMessageBuilder messageBuilder;
    const std::string expectedPeerId = "twentyBytesPeerId---";

    messageBuilder.setPeerId(expectedPeerId);

    const std::string handshake = messageBuilder.getMessage();

    EXPECT_EQ(handshake.size(), expectedMessageSize);
    EXPECT_EQ(handshake.substr(48,20), expectedPeerId);
}

TEST(HandshakeMessageBuilder, setPeerIdLongerThanTwentyBytes) {
    const std::size_t expectedMessageSize = 68;
    HandshakeMessageBuilder messageBuilder;
    const std::string expectedPeerId = "twentyBytesPeerId---";
    const std::string peerId = expectedPeerId + 'a';

    messageBuilder.setPeerId(peerId);

    const std::string handshake = messageBuilder.getMessage();

    EXPECT_EQ(handshake.size(), expectedMessageSize);
    EXPECT_EQ(handshake.substr(48,20), expectedPeerId);
}

TEST(HandshakeMessageBuilder, setPeerIdShorterThanTwentyBytes) {
    const std::size_t expectedMessageSize = 68;
    HandshakeMessageBuilder messageBuilder;
    const std::string peerId = "InfoHash";
    std::string expectedPeerId = peerId;
    while(expectedPeerId.size() < 20)
        expectedPeerId.push_back('-');

    messageBuilder.setPeerId(peerId);

    const std::string handshake = messageBuilder.getMessage();

    EXPECT_EQ(handshake.size(), expectedMessageSize);
    EXPECT_EQ(handshake.substr(48,20), expectedPeerId);
}
