#include "Bittorrent/TrackerResponse.hpp"

#include <cinttypes>

TrackerResponse::TrackerResponse(std::string const& data): CanBeBuiltFromBencodeData(data) {
    if(!m_bencodeValues["interval"].empty())
        m_interval = static_cast<std::uint32_t>(std::stoul(m_bencodeValues["interval"]));
    m_peers = m_bencodeValues["peers"];
}

std::vector<std::pair<std::string, char>> TrackerResponse::getFieldsToFill() const {
    return {{"interval", 'v'}, {"peers", 'v'}};
}

std::vector<Peer> TrackerResponse::getPeers() {
    const std::size_t peersSize = m_peers.size();
    std::vector<Peer> peers;
    for (std::size_t i = 0; i < m_peers.size(); i += 6) {
        std::stringstream address;
        std::stringstream port;
        address << static_cast<int>(static_cast<uint8_t>(m_peers[i])) << '.';
        address << static_cast<int>(static_cast<uint8_t>(m_peers[i + 1])) << '.';
        address << static_cast<int>(static_cast<uint8_t>(m_peers[i + 2])) << '.';
        address << static_cast<int>(static_cast<uint8_t>(m_peers[i + 3]));

        std::uint16_t portNumber = static_cast<uint8_t>(m_peers[i + 4]);
        portNumber = portNumber << 8;
        portNumber += static_cast<uint8_t>(m_peers[i + 5]);

        peers.push_back(Peer::fromAddressAndPort(address.str(), portNumber));
    }
    return peers;
}

std::uint32_t TrackerResponse::getInterval() {
    return m_interval;
}
