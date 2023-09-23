#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"
#include <cmath>

/*!
 * @brief class represents 'request' message from BitTorrent protocol.
*/

class Request: public IBittorrentMessageBuildable<MessageType::REQUEST> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::REQUEST;
    static constexpr int DEFAULT_LENGTH = std::pow(2, 14);
public:
    explicit Request(int index, int begin, int length = DEFAULT_LENGTH) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(index, begin, length);
    }
};
