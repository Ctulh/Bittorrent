//
// created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Port: public IBittorrentMessageBuildable<MessageType::PORT> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::PORT;

public:
    explicit Port(unsigned short port) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(port);
    }
};
