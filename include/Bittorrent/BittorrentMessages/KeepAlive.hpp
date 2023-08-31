#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

/*!
 * @brief class represents 'keep-alive' message from BitTorrent protocol.
*/

class KeepAlive: public IBittorrentMessageBuildable<MessageType::KEEP_ALIVE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::KEEP_ALIVE;

public:
    KeepAlive() = default;
};
