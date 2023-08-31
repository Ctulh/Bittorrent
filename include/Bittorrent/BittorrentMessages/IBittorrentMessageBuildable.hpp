#pragma once

#include "BittorrentMessageType.hpp"
#include "BittorrentMessageBuilder.hpp"
#include "BittorrentMessage.hpp"

template<MessageType messageTypeValue>
class IBittorrentMessageBuildable: public BittorrentMessage {
public:
    IBittorrentMessageBuildable(): BittorrentMessage(messageTypeValue) {}
public:
    virtual std::vector<std::byte> getMessage() const {
        BittorrentMessageBuilder builder;
        builder.setMessageType(messageTypeValue);
        builder.setPayload(m_payload);
        return builder.getMessage();
    }
};
