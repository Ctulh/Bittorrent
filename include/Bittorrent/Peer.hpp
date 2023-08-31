#pragma once

#include "PeerInfo.hpp"
#include "Socket.hpp"
#include "StreamSocket.hpp"

#include <boost/asio.hpp>
#include <memory>

/*!
 * @brief class that represents peer.
 */
class Peer {
public:
    Peer(PeerInfo const& peerInfo);
public:
    /*!
     * @param request
     * @return true - if handshake is successful, or false if not.
     */
    bool handshake(std::string const& request);
    StreamSocketPtr getSocket() {
        return m_streamSocket;
    }
private:
    PeerInfo m_peerInfo;
    StreamSocketPtr m_streamSocket;
};
