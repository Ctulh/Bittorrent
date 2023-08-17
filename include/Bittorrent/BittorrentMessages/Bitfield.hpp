//
// Created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessage.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Bitfield: public IBittorrentMessage<MessageType::BITFIELD> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::BITFIELD;

public:
    explicit Bitfield(std::vector<std::byte> const& bitfield) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(bitfield);
    }
};
