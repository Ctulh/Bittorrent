//
// Created by ctuh on 8/12/23.
//

#include "BittorrentMessages/BittorrentMessageBuilder.hpp"

#include <sstream>

BittorrentMessageBuilder::BittorrentMessageBuilder() {
    m_messageTypesWithPayload = {MessageType::HAVE, MessageType::BITFIELD, MessageType::REQUEST, MessageType::PIECE, MessageType::CANCEL, MessageType::PORT};
}
void BittorrentMessageBuilder::setMessageType(MessageType messageType) {
    m_messageType = messageType;
}

void BittorrentMessageBuilder::setPayload(std::string const& payload) {
    m_payload = payload;
}
std::string BittorrentMessageBuilder::getMessage() const {
    if(m_messageType == MessageType::KEEP_ALIVE) {
        return "0";
    }
    else if(m_messageTypesWithPayload.contains(m_messageType)) {
        return buildStringForPayload();
    }
    return buildStringForMessageWithoutPayload();
}

std::string BittorrentMessageBuilder::buildStringForPayload() const {
    std::stringstream messageStream;
    messageStream << m_payload.size() + 1 << static_cast<int>(m_messageType) << m_payload;
    return messageStream.str();
}

std::string BittorrentMessageBuilder::buildStringForMessageWithoutPayload() const {
    std::stringstream messageStream;
    messageStream << 1 << static_cast<int>(m_messageType);
    return messageStream.str();
}
