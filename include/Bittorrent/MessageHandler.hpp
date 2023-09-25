//
// Created by ctuh on 9/25/23.
//

#pragma once

#include <functional>
#include <string>
#include <vector>
#include <unordered_map>

#include "BittorrentMessages/BittorrentMessageType.hpp"
#include "BittorrentMessages/BittorrentMessage.hpp"

using BitTorrentMessageHandlerType = std::function<void(std::string const& host, std::vector<std::byte> const& payload )>;

class MessageHandler {
public:
    void setHandler(MessageType messageType, BitTorrentMessageHandlerType handler);
    void handle(std::string const& host, BittorrentMessage const& message);

private:
    std::unordered_map<MessageType, BitTorrentMessageHandlerType> m_typeToHandler;
};