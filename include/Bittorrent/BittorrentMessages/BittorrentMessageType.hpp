//
// Created by ctuh on 8/12/23.
//

#pragma once

#include <cinttypes>

enum class MessageType {
    CHOKE = 0,
    UNCHOKE = 1,
    INTERESTED = 2,
    NOT_INTERESTED = 3,
    HAVE = 4,
    BITFIELD = 5,
    REQUEST = 6,
    PIECE = 7,
    CANCEL = 8,
    PORT = 9,
    KEEP_ALIVE = 10,
};
