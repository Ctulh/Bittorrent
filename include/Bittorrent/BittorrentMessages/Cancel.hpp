//
// Created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessage.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Cancel: public IBittorrentMessage<MessageType::CANCEL> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::CANCEL;

public:
    explicit Cancel(int index, int begin, int length) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(index, begin, length);
    }
};
