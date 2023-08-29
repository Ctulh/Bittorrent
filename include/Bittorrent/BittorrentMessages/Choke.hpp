//
// Created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Choke: public IBittorrentMessageBuildable<MessageType::CHOKE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::CHOKE;

public:
    Choke() = default;
};
