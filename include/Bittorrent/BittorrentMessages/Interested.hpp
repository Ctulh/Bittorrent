#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

/*!
 * @brief class represents 'interested' message from BitTorrent protocol.
*/

class Interested: public IBittorrentMessageBuildable<MessageType::INTERESTED> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::INTERESTED;

public:
    Interested() = default;
};
