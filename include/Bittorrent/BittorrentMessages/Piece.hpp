#pragma once

#include "IBittorrentMessageBuildable.hpp"
#include "ConverterArgsToPayloadString.hpp"

/*!
 * @brief class represents 'piece' message from BitTorrent protocol.
*/

class Piece: public IBittorrentMessageBuildable<MessageType::PIECE> {
public:
    static constexpr MessageType MESSAGE_TYPE = MessageType::PIECE;

public:
    explicit Piece(int index, int begin, std::vector<std::byte> const& block) {
        m_payload = ConverterArgsToPayloadString<MESSAGE_TYPE>::getPayload(index, begin, block);
    }
};
