//
// created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Request: public IBittorrentMessageBuildable<MessageType::REQUEST> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::REQUEST;

public:
    explicit Request(int index, int begin, int length) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(index, begin, length);
    }
};
