#pragma once

#include <string>

class HandshakeMessageBuilder {
public:
    HandshakeMessageBuilder();

public:
    HandshakeMessageBuilder& setPeerId(std::string const& peerId);
    HandshakeMessageBuilder& setInfoHash(std::string const& infoHash);
    HandshakeMessageBuilder& setReserved(std::string const& reservedBytes);

    std::string getMessage() const;

private:
    static std::size_t reservedLength();
    static std::size_t infoHashLength();
    static std::size_t peerIdLength();

private:
    const std::string protocolStr = "BitTorrent protocol";
    std::string m_reservedBytes;
    std::string m_infoHash;
    std::string m_peerId;
    const char m_extender = '-';
};
