//
// Created by ctuh on 8/12/23.
//

#include "Bittorrent/BittorrentMessageBuilder.hpp"

#include <sstream>

std::string BittorrentMessageBuilder::getMessage() const {
    std::stringstream messageStream;
    messageStream << m_payload.size() + 1 << static_cast<int>(m_messageType) << m_payload;
    return messageStream.str();
}

void BittorrentMessageBuilder::setMessageType(MessageType messageType) {
    m_messageType = messageType;
}

void BittorrentMessageBuilder::setPayload(std::string const& payload) {
    m_payload = payload;
}
