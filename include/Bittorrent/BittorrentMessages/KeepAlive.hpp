//
// created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessage.hpp"
#include "ConverterArgsToPayloadString.hpp"

class KeepAlive: public IBittorrentMessage<MessageType::KEEP_ALIVE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::KEEP_ALIVE;

public:
    KeepAlive() = default;
};
