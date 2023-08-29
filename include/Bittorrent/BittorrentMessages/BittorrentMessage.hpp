#pragma once

#include "BittorrentMessageType.hpp"
#include <vector>
#include <ostream>

class BittorrentMessage {
public:
    BittorrentMessage(MessageType messageType, std::vector<std::byte> const& payload): m_payload(payload),
                                                                                       m_messageType(messageType) {}
    BittorrentMessage(MessageType messageType): m_messageType(messageType) {}
    BittorrentMessage(): m_messageType(MessageType::KEEP_ALIVE) {}

    virtual ~BittorrentMessage() = default;

public:
    virtual MessageType getMessageType() const;
    virtual std::vector<std::byte> getPayload() const;
    virtual void setMessageType(MessageType messageType);
    virtual void setPayload(std::vector<std::byte> const& payload);

public:
    friend std::ostream& operator<<(std::ostream & os, BittorrentMessage const& bittorrentMessage);

protected:
    std::vector<std::byte> m_payload;
    MessageType m_messageType;
};
