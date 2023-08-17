//
// created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessage.hpp"
#include "ConverterArgsToPayloadString.hpp"

class NotInterested: public IBittorrentMessage<MessageType::NOT_INTERESTED> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::NOT_INTERESTED;

public:
    NotInterested() = default;
};
