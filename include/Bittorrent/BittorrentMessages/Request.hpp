//
// created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessage.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Request: public IBittorrentMessage<MessageType::REQUEST> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::REQUEST;

public:
    explicit Request(int index, int begin, int length) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(index, begin, length);
    }
};
