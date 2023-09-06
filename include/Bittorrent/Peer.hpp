#pragma once

#include "PeerInfo.hpp"
#include "Socket.hpp"
#include "StreamSocket.hpp"

#include <boost/asio.hpp>
#include <memory>
#include <unordered_set>

/*!
 * @brief class that represents peer.
 */
class Peer {
public:
    Peer(PeerInfo const& peerInfo);
    Peer(Peer const& peer) = default;
    Peer& operator=(Peer const& peer) = default;
public:
    /*!
     * @param request
     * @return true - if handshake is successful, or false if not.
     */
    bool handshake(std::string const& request);
    StreamSocketPtr getSocket() {
        return m_streamSocket;
    }

    void addPieces(std::vector<std::size_t> const& pieces);
    bool hasPiece(std::size_t piece) const;
private:
    std::unordered_set<std::size_t> m_pieces;
    PeerInfo m_peerInfo;
    StreamSocketPtr m_streamSocket;
};
