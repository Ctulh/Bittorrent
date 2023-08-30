#include "Bittorrent/BittorrentMessages/HandshakeMessageBuilder.hpp"
#include "Bittorrent/BitTorrentIdentity.hpp"
#include "Logger/Logger.hpp"

#include <format>

HandshakeMessageBuilder::HandshakeMessageBuilder(): m_peerId(BitTorrentIdentity::getPeerId()) {
    m_reservedBytes.resize(reservedLength(), 0x0);
    m_infoHash.resize(infoHashLength(), 0x0);
}

HandshakeMessageBuilder& HandshakeMessageBuilder::setReserved(std::string const& reserved) {
    if(reserved.size() > reservedLength()) {
        auto reservedTruncated = reserved.substr(0, reservedLength());
        Logger::logWarning(std::format(
                "Got 'reserved' string larger than expected in HandshakeMessageBuilder. Got: {}, will be truncated into: {}",
                reserved, reservedTruncated));
        m_reservedBytes = reservedTruncated;
    }
    else if(reserved.size() < reservedLength()) {
        std::string reservedExtended = reserved;
        while(reservedExtended.size() < reservedLength()) {
            reservedExtended.push_back(0x0);
        }
        Logger::logWarning(std::format(
                "Got 'reserved' shorter shorter than expected in HandshakeMessageBuilder. Got: {}, will be extended into: {}",
                reserved, reservedExtended));
        m_reservedBytes = reservedExtended;
    }
    else {
        m_reservedBytes = reserved;
    }
    return *this;
}

HandshakeMessageBuilder& HandshakeMessageBuilder::setInfoHash(std::string const& infoHash) {
    if(infoHash.size() > infoHashLength()) {
        auto infoHashTruncated = infoHash.substr(0, infoHashLength());
        Logger::logWarning(std::format(
                "Got 'infoHash' string larger than expected in HandshakeMessageBuilder. Got: {}, will be truncated into: {}",
                infoHash, infoHashTruncated));
        m_infoHash = infoHashTruncated;
    }
    else if(infoHash.size() < infoHashLength()) {
        Logger::logWarning(std::format(
                "Got 'infoHash' string shorter than expected in HandshakeMessageBuilder. Got: {}. infoHash value will not changed.",
                infoHash));
    }
    else {
        m_infoHash = infoHash;
    }
    return *this;
}

HandshakeMessageBuilder& HandshakeMessageBuilder::setPeerId(std::string const& peerId) {
    if(peerId.size() > peerIdLength()) {
        auto peerIdTruncated = peerId.substr(0, peerIdLength());
        Logger::logWarning(std::format(
                "Got 'peerId' string larger than expected in HandshakeMessageBuilder. Got: {}, will be truncated into: {}",
                peerId, peerIdTruncated));
        m_peerId = peerIdTruncated;
    }
    else if(peerId.size() < peerIdLength()) {
        auto peerIdExtended = peerId;
        while (peerIdExtended.size() < peerIdLength()) {
            peerIdExtended.push_back(m_extender);
        }
        Logger::logWarning(std::format(
                "Got 'peerId' string shorter than expected in HandshakeMessageBuilder. Got: {}, will be extended into: {}",
                peerId, peerIdExtended));
        m_peerId = peerIdExtended;
    }
    else {
        m_peerId = peerId;
    }
    return *this;
}

std::string HandshakeMessageBuilder::getMessage() const {
    std::string message;
    message.push_back(0x13);
    message += protocolStr;
    message += m_reservedBytes;
    message += m_infoHash;
    message += m_peerId;
    return message;
}

std::size_t HandshakeMessageBuilder::infoHashLength() {
    return 20;
}

std::size_t HandshakeMessageBuilder::peerIdLength() {
    return 20;
}

std::size_t HandshakeMessageBuilder::reservedLength() {
    return 8;
}
