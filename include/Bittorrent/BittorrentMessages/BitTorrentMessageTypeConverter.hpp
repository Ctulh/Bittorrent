//
// Created by ctuh on 9/23/23.
//

#pragma once

#include "BittorrentMessageType.hpp"

#include <string>
#include <unordered_map>

class BitTorrentMessageTypeConverter {
public:
    BitTorrentMessageTypeConverter() = delete;

public:
    static std::string messageTypeToString(MessageType messageType);
    static MessageType stringToMessageType(std::string const& messageTypeStr);

private:
    static std::unordered_map<MessageType, std::string> const& getTypeToStringRelation();
};
