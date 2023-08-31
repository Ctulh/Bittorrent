#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

/*!
 * @brief class represents 'request' message from BitTorrent protocol.
*/

class Request: public IBittorrentMessageBuildable<MessageType::REQUEST> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::REQUEST;

public:
    explicit Request(int index, int begin, int length) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(index, begin, length);
    }
};
