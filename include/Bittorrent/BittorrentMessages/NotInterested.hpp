#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

/*!
 * @brief class represents 'not interested' message from BitTorrent protocol.
*/

class NotInterested: public IBittorrentMessageBuildable<MessageType::NOT_INTERESTED> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::NOT_INTERESTED;

public:
    NotInterested() = default;
};
