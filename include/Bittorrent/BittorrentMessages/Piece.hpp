//
// created by ctuh on 8/12/23.
//

#pragma once

#include "IBittorrentMessage.hpp"
#include "ConverterArgsToPayloadString.hpp"

class Piece: public IBittorrentMessage<MessageType::PIECE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::PIECE;

public:
    explicit Piece(int index, int begin, std::vector<std::byte> const& block) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(index, begin, block);
    }
};
