#pragma once

#include <string>

/*!
 * @brief Class that build handshake message between client and peer.
 */

class HandshakeMessageBuilder {
public:
    HandshakeMessageBuilder();

public:
/*!
 * @param reservedBytes - string with eight (8) reserved bytes. All current BitTorrent protocol implementations use all zeroes.
 *        By default this string with eight zeros.
 *        You can learn more at the BitTorrent protocol specification
 * @return current object reference.
 */
    HandshakeMessageBuilder& setReserved(std::string const& reservedBytes);

/*!
 * @param infoHash - 20-byte SHA1 hash of the info key in the metainfo file.
 *        If this parameter is longer than twenty bytes only the first twenty bytes will be used.
 *        You can learn more at the BitTorrent protocol specification
 * @return current object reference.
*/
    HandshakeMessageBuilder& setInfoHash(std::string const& infoHash);

/*!
 * @param peerId - 20-byte string used as a unique ID for the client.
 *        If this parameter is longer than twenty bytes only the first twenty bytes will be used.
 *        If this parameter is shorter that twenty bytes 'peerId' will be extended from the end with character from 'HandshakeMessageBuilder::extender()'.
 *        You can learn more at the BitTorrent protocol specification
 * @return
 */
    HandshakeMessageBuilder& setPeerId(std::string const& peerId);

/*!
 * @return string that can be sent to the client and interpreted as a bittorrent protocol handshake.
*/
    std::string getMessage() const;

private:
/*!
 * @return length of the 'reserved' part in BitTorrent protocol. Now this value is 8.
*/
    static std::size_t reservedLength();

/*!
 * @return length of the 'infoHash' part in BitTorrent protocol. Now this value is 20.
*/
    static std::size_t infoHashLength();

/*!
 * @return length of the 'peerId' part in BitTorrent protocol. Now this value is 20.
*/
    static std::size_t peerIdLength();

/*!
 * @return string identifier of the BitTorrent protocol. Now this value is "BitTorrent protocol".
*/
    static std::string protocolStr();

/*!
* @return character that will used for extend peerId if it shorter than 'peerIdLength()'.
*/
    static char extender();

private:
    std::string m_reservedBytes;
    std::string m_infoHash;
    std::string m_peerId;
};
