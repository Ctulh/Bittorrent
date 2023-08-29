//
// created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

class KeepAlive: public IBittorrentMessageBuildable<MessageType::KEEP_ALIVE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::KEEP_ALIVE;

public:
    KeepAlive() = default;
};
