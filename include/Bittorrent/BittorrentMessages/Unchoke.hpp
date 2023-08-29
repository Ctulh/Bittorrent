//
// created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Unchoke: public IBittorrentMessageBuildable<MessageType::UNCHOKE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::UNCHOKE;

public:
    Unchoke() = default;
};
