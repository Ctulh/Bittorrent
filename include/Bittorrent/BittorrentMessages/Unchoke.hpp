#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

/*!
 * @brief class represents 'choke' message from BitTorrent protocol.
*/

class Unchoke: public IBittorrentMessageBuildable<MessageType::UNCHOKE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::UNCHOKE;

public:
    Unchoke() = default;
};
