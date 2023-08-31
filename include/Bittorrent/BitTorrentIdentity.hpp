#pragma once

#include <string>

/*!
 * @brief Class that stores service information for client.
 */

class BitTorrentIdentity {
public:
    BitTorrentIdentity() = delete;

public:
/*!
  * @return peerId - almost any string with 20 bytes length is fine.
*/
    static std::string getPeerId();
};
