#pragma once

#include "PeerStatus.hpp"
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
    Peer(Peer const& peer) = default;
    Peer& operator=(Peer const& peer) = default;
private:
    Peer(std::string const& address, unsigned short port);
public:
    static Peer fromAddressAndPort(std::string const& address, unsigned short port);
    /*!
     * @param request
     * @return true - if handshake is successful, or false if not.
     */
    bool handshake(std::string const& request) const;
    StreamSocketPtr getSocket() const;

    void addPieces(std::vector<std::size_t> const& pieces);
    bool hasPiece(std::size_t piece) const;

    void setStatus(PeerStatus status);
    PeerStatus getStatus() const;
    std::string getAddress() const;
    unsigned short getPort() const;
private:
    PeerStatus m_status;
    std::string m_address;
    unsigned short m_port;
    std::unordered_set<std::size_t> m_pieces;
    StreamSocketPtr m_streamSocket;
};
