//
// created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessage.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Interested: public IBittorrentMessage<MessageType::INTERESTED> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::INTERESTED;

public:
    Interested() = default;
};
