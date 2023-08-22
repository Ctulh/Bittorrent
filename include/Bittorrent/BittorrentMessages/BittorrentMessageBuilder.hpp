//
// Created by ctuh on 8/12/23.
//

#pragma once

#include <string>
#include <vector>

#include "BittorrentMessageType.hpp"
#include <unordered_set>

class BittorrentMessageBuilder {
public:
    BittorrentMessageBuilder();

public:
    void setMessageType(MessageType messageType);
    void setPayload(std::vector<std::byte> const& payload);
    std::vector<std::byte> getMessage() const;

private:
    std::vector<std::byte> buildStringForPayload() const;
    std::vector<std::byte> buildStringForMessageWithoutPayload() const;

private:
    std::unordered_set<MessageType> m_messageTypesWithPayload;
    std::vector<std::byte> m_payload;
    MessageType m_messageType = MessageType::KEEP_ALIVE;
};
