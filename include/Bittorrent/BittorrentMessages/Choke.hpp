#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

/*!
 * @brief class represents 'choke' message from BitTorrent protocol.
*/

class Choke: public IBittorrentMessageBuildable<MessageType::CHOKE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::CHOKE;

public:
    Choke() = default;
};
