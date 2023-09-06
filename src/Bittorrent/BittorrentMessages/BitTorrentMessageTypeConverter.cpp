//
// Created by ctuh on 9/5/23.
//

#include "Bittorrent/BittorrentMessages/BitTorrentMessageTypeConverter.hpp"

#include <algorithm>

std::string BitTorrentMessageTypeConverter::messageTypeToString(MessageType messageType) {
    return getTypeToStringRelation().at(messageType);
}

MessageType BitTorrentMessageTypeConverter::stringToMessageType(std::string const& messageTypeStr) {
    auto typeToStringRelation = getTypeToStringRelation();

    auto isStringsEqual = [&messageTypeStr](std::pair<MessageType, std::string> const& relation) {
        return messageTypeStr == relation.second;
    };

    auto it = std::find_if(typeToStringRelation.begin(), typeToStringRelation.end(), isStringsEqual);
    if(it != typeToStringRelation.end())
        return it->first;
    return MessageType::KEEP_ALIVE;
}

const std::unordered_map<MessageType, std::string> &BitTorrentMessageTypeConverter::getTypeToStringRelation() {
    static std::unordered_map<MessageType, std::string> typeToStringRelation {
            {MessageType::CHOKE, "CHOKE"},
            {MessageType::UNCHOKE, "UNCHOKE"},
            {MessageType::INTERESTED, "INTERESTED"},
            {MessageType::NOT_INTERESTED, "NOT_INTERESTED"},
            {MessageType::HAVE, "HAVE"},
            {MessageType::PIECE, "PIECE"},
            {MessageType::REQUEST, "REQUEST"},
            {MessageType::PORT, "PORT"},
            {MessageType::KEEP_ALIVE, "KEEP_ALIVE"},
            {MessageType::BITFIELD, "BITFIELD"},
    };
    return typeToStringRelation;
}
