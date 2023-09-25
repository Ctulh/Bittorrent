//
// Created by ctuh on 9/25/23.
//

#pragma once

#include "Bittorrent/MessageHandler.hpp"

#include <gtest/gtest.h>

TEST(MessageHandlerTest, ConstructMessageHandler) {
    ASSERT_NO_THROW(MessageHandler());
}

TEST(MessageHandlerTest, HandleMessageWithoutHandler) {
    MessageHandler messageHandler;
    BittorrentMessage message(MessageType::KEEP_ALIVE);

    ASSERT_NO_THROW(messageHandler.handle("", message));
}

TEST(MessageHandlerTest, HandleMessageWithHandler) {
    MessageHandler messageHandler;
    BittorrentMessage message(MessageType::KEEP_ALIVE);
    bool flag = false;
    auto handler = [&flag](std::string const&, std::vector<std::byte> const&) {
        flag = true;
    };

    messageHandler.setHandler(message.getMessageType(), handler);

    ASSERT_FALSE(flag);
    ASSERT_NO_THROW(messageHandler.handle("", message));
    ASSERT_TRUE(flag);
}

TEST(MessageHandlerTest, RedefineHandler) {
    MessageHandler messageHandler;
    BittorrentMessage message(MessageType::KEEP_ALIVE);
    std::size_t value = 0;
    auto firstHandler = [&value](std::string const&, std::vector<std::byte> const&) {
        value = 1;
    };
    auto secondHandler = [&value](std::string const&, std::vector<std::byte> const&) {
        value = -1;
    };

    messageHandler.setHandler(message.getMessageType(), firstHandler);

    ASSERT_TRUE(value == 0);
    messageHandler.handle("", message);
    ASSERT_TRUE(value == 1);

    messageHandler.setHandler(message.getMessageType(), secondHandler);

    messageHandler.handle("", message);
    ASSERT_TRUE(value == -1);
}

TEST(MessageHandlerTest, HandleMultipleMessageTypes) {
    MessageHandler messageHandler;
    BittorrentMessage firstMessage(MessageType::KEEP_ALIVE);
    BittorrentMessage secondMessage(MessageType::HAVE);
    std::size_t value = 0;
    auto firstHandler = [&value](std::string const&, std::vector<std::byte> const&) {
        value = 1;
    };
    auto secondHandler = [&value](std::string const&, std::vector<std::byte> const&) {
        value = -1;
    };

    messageHandler.setHandler(firstMessage.getMessageType(), firstHandler);
    messageHandler.setHandler(secondMessage.getMessageType(), secondHandler);

    ASSERT_TRUE(value == 0);
    messageHandler.handle("", firstMessage);
    ASSERT_TRUE(value == 1);
    messageHandler.handle("", secondMessage);
    ASSERT_TRUE(value == -1);
}
