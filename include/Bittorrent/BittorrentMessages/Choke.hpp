//
// Created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessage.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Choke: public IBittorrentMessage<MessageType::CHOKE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::CHOKE;

public:
    Choke() = default;
};
