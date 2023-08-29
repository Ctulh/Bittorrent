//
// Created by ctuh on 8/25/23.
//

#pragma once

#include "Bittorrent/BittorrentMessages/BittorrentMessage.hpp"

#include <gtest/gtest.h>

TEST(BittorrentMessageTests, constructBittorrnetMessage) {
    EXPECT_NO_THROW(BittorrentMessage());
}

TEST(BittorrentMessageTests, getPayloadFromDefaultConstructedMessage) {
    BittorrentMessage bittorrentMessage;

    EXPECT_TRUE(bittorrentMessage.getPayload().empty());
}

TEST(BittorrentMessageTests, getMessageTypeFromDefaultConstructedMessage) {
    BittorrentMessage bittorrentMessage;

    EXPECT_EQ(bittorrentMessage.getMessageType(), MessageType::KEEP_ALIVE);
}

TEST(BittorrentMessageTests, contructMessageFromMessagType) {
    BittorrentMessage bittorrentMessage(MessageType::INTERESTED);
}

TEST(BittorrentMessageTests, getPayloadFromConctructedWithMessageType) {
    BittorrentMessage bittorrentMessage(MessageType::INTERESTED);

    EXPECT_TRUE(bittorrentMessage.getPayload().empty());
}

TEST(BittorrentMessageTests, getMessageTypeFromConctructedWithMessageType) {
    BittorrentMessage bittorrentMessage(MessageType::INTERESTED);

    EXPECT_EQ(bittorrentMessage.getMessageType(), MessageType::INTERESTED);
}

TEST(BittorrentMessageTests, contructMessageFromPayloadAndMessageType) {
    BittorrentMessage bittorrentMessage(MessageType::HAVE, std::vector<std::byte>{std::byte(0x1)});
}

TEST(BittorrentMessageTests, getPayloadFromMessageContructedWithPayloadAndType) {
    BittorrentMessage bittorrentMessage(MessageType::HAVE, std::vector<std::byte>{std::byte(0x1)});

    EXPECT_EQ(bittorrentMessage.getPayload().back(), std::byte(0x1));
}

TEST(BittorrentMessageTests, getMessageTypeFromMessageContructedWithPayloadAndType) {
    BittorrentMessage bittorrentMessage(MessageType::HAVE, std::vector<std::byte>{std::byte(0x1)});

    EXPECT_EQ(bittorrentMessage.getMessageType(), MessageType::HAVE);
}

TEST(BittorrentMessageTests, setMessageType) {
    BittorrentMessage bittorrentMessage;

    bittorrentMessage.setMessageType(MessageType::CHOKE);

    EXPECT_EQ(bittorrentMessage.getMessageType(), MessageType::CHOKE);
}

TEST(BittorrentMessageTests, setPayload) {
    BittorrentMessage bittorrentMessage;

    bittorrentMessage.setPayload(std::vector<std::byte>{std::byte(0x1)});

    EXPECT_EQ(bittorrentMessage.getPayload().back(), std::byte(0x1));
}
