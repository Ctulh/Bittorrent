//
// Created by ctuh on 8/12/23.
//

#pragma once

#include <string>

#include "BittorrentMessageType.hpp"
#include <unordered_set>

class BittorrentMessageBuilder {
public:
    BittorrentMessageBuilder();

public:
    void setMessageType(MessageType messageType);
    void setPayload(std::string const& payload);
    std::string getMessage() const;

private:
    std::string buildStringForPayload() const;
    std::string buildStringForMessageWithoutPayload() const;

private:
    std::unordered_set<MessageType> m_messageTypesWithPayload;
    std::string m_payload;
    MessageType m_messageType = MessageType::KEEP_ALIVE;
};
