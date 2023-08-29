#include "Bittorrent/BittorrentMessages/BittorrentMessage.hpp"

#include <sstream>

std::vector<std::byte> BittorrentMessage::getPayload() const {
    return m_payload;
}

MessageType BittorrentMessage::getMessageType() const {
    return m_messageType;
}

void BittorrentMessage::setPayload(std::vector<std::byte> const& payload) {
    m_payload = payload;
}

void BittorrentMessage::setMessageType(MessageType messageType) {
    m_messageType = messageType;
}

std::ostream& operator<<(std::ostream& os, BittorrentMessage const& bittorrentMessage) {
    std::stringstream payloadStream;
    for(auto el: bittorrentMessage.m_payload) {
        payloadStream << std::hex << static_cast<uint8_t>(el);
    }
    os << static_cast<int>(bittorrentMessage.m_messageType) << ": " << payloadStream.str();
    return os;
}
