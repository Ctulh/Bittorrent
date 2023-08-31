#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

/*!
 * @brief class represents 'port' message from BitTorrent protocol.
*/

class Port: public IBittorrentMessageBuildable<MessageType::PORT> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::PORT;

public:
    explicit Port(unsigned short port) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(port);
    }
};
