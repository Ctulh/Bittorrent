//
// Created by ctuh on 8/12/23.
//

#pragma once

#include <string>
#include <vector>

#include "BittorrentMessageType.hpp"
#include <unordered_set>

/*!
 * @brief Class that helps create messages between a client and a peer.
*/

class BittorrentMessageBuilder {
public:
    BittorrentMessageBuilder();

public:
/*!
 * @brief method has inner logic, that means even if you already set payload for 'messageType' without payload part in
 *       BitTorrent protocol payload part will be ignored, and you will get always correct message. See tests for clarification.
 * @param messageType - message type to be built.
*/
    void setMessageType(MessageType messageType);

/*!
 * @brief method has inner logic, that means even if you already set 'messageType' without payload part in
 *        BitTorrent protocol payload part will be ignored, and you will get always correct message. See tests for clarification.
 * @param payload - raw data for message.
*/
    void setPayload(std::vector<std::byte> const& payload);

/*!
 * @return raw message that can be sent to the peer.
*/
    std::vector<std::byte> getMessage() const;

private:
    std::vector<std::byte> buildStringForPayload() const;
    std::vector<std::byte> buildStringForMessageWithoutPayload() const;

private:
    std::unordered_set<MessageType> m_messageTypesWithPayload;
    std::vector<std::byte> m_payload;
    MessageType m_messageType = MessageType::KEEP_ALIVE;
};
