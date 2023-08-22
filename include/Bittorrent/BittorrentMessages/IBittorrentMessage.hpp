//
// created by ctuh on 8/12/23.
//

#pragma once

#include "BittorrentMessageType.hpp"
#include "BittorrentMessageBuilder.hpp"

template<MessageType messageTypeValue>
class IBittorrentMessage {
public:
    virtual ~IBittorrentMessage() = default;

public:
    virtual std::vector<std::byte> getMessage() const {
        BittorrentMessageBuilder builder;
        builder.setMessageType(messageTypeValue);
        builder.setPayload(m_payload);
        return builder.getMessage();
    }
protected:
    std::vector<std::byte> m_payload;
};