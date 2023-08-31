#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

/*!
 * @brief class represents 'have' message from BitTorrent protocol.
*/

class Have: public IBittorrentMessageBuildable<MessageType::HAVE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::HAVE;

public:
    explicit Have(int pieceIndex) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(pieceIndex);
    }
};

