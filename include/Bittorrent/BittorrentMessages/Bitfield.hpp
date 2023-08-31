#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

/*!
 * @brief class represents 'bitfield' message from BitTorrent protocol.
*/

class Bitfield: public IBittorrentMessageBuildable<MessageType::BITFIELD> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::BITFIELD;

public:
    explicit Bitfield(std::vector<std::byte> const& bitfield) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(bitfield);
    }
};
