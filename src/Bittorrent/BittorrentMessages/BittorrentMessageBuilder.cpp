//
// Created by ctuh on 8/12/23.
//

#include "BittorrentMessages/BittorrentMessageBuilder.hpp"
#include "Utils/ByteMethods.hpp"

#include <cmath>

BittorrentMessageBuilder::BittorrentMessageBuilder() {
    m_messageTypesWithPayload = {MessageType::HAVE, MessageType::BITFIELD, MessageType::REQUEST, MessageType::PIECE, MessageType::CANCEL, MessageType::PORT};
}
void BittorrentMessageBuilder::setMessageType(MessageType messageType) {
    m_messageType = messageType;
}

void BittorrentMessageBuilder::setPayload(std::vector<std::byte> const& payload) {
    m_payload = payload;
}
std::vector<std::byte> BittorrentMessageBuilder::getMessage() const {
    if(m_messageType == MessageType::KEEP_ALIVE) {
        return {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x0)};
    }
    else if(m_messageTypesWithPayload.contains(m_messageType)) {
        return buildStringForPayload();
    }
    return buildStringForMessageWithoutPayload();
}

std::vector<std::byte> BittorrentMessageBuilder::buildStringForPayload() const {
    std::vector<std::byte> message;
    std::size_t size = m_payload.size() + 1;

    auto sizeBytes = ByteMethods::convertNumberToFourBytes(size);

    message.insert(message.begin(), sizeBytes.begin(), sizeBytes.end());
    message.push_back(static_cast<std::byte>(m_messageType));
    message.insert(message.end(), m_payload.begin(), m_payload.end());
    return message;
}

std::vector<std::byte> BittorrentMessageBuilder::buildStringForMessageWithoutPayload() const {
    std::vector<std::byte> message {std::byte(0x0), std::byte(0x0), std::byte(0x0), std::byte(0x1)};
    message.push_back(static_cast<std::byte>(m_messageType));
    return message;
}
