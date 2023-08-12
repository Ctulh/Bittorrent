//
// Created by ctuh on 8/12/23.
//

#pragma once

#include <string>

#include "BittorrentMessageType.hpp"

class BittorrentMessageBuilder {
public:
    void setMessageType(MessageType messageType);
    void setPayload(std::string const& payload);
    std::string getMessage() const;

private:
    std::string m_payload;
    MessageType m_messageType;
};
