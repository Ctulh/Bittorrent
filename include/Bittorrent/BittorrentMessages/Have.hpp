//
// Created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessage.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Have: public IBittorrentMessage<MessageType::HAVE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::HAVE;

public:
    explicit Have(int pieceIndex) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(pieceIndex);
    }
};

