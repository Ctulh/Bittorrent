//
// created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Interested: public IBittorrentMessageBuildable<MessageType::INTERESTED> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::INTERESTED;

public:
    Interested() = default;
};
