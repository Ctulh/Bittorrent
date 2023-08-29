//
// created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

class NotInterested: public IBittorrentMessageBuildable<MessageType::NOT_INTERESTED> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::NOT_INTERESTED;

public:
    NotInterested() = default;
};
