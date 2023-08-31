#pragma once

#include "BittorrentMessageType.hpp"
#include <vector>
#include <ostream>

/*!
 * @brief class represents the message between client and peer.
*/

class BittorrentMessage {
public:
    BittorrentMessage(MessageType messageType, std::vector<std::byte> const& payload);
    explicit BittorrentMessage(MessageType messageType);
    BittorrentMessage();

    virtual ~BittorrentMessage() = default;

public:
    [[nodiscard]] virtual MessageType getMessageType() const;
    [[nodiscard]] virtual std::vector<std::byte> getPayload() const;
    virtual void setMessageType(MessageType messageType);
    virtual void setPayload(std::vector<std::byte> const& payload);

public:
    friend std::ostream& operator<<(std::ostream & os, BittorrentMessage const& bittorrentMessage);

protected:
    std::vector<std::byte> m_payload;
    MessageType m_messageType;
};
