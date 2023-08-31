#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <format>

#include "BittorrentMessage.hpp"
#include "Utils/ByteMethods.hpp"
#include "Logger/Logger.hpp"

/*!
 * @brief Class that parse message from peer.
*/

class BittorrentMessageParser {
public:
    /*!
     * @param message - raw message from peer.
     * @return vector of BitTorrentMessage parsed from 'message'. Vector can be empty if there is no BitTorrentMessages
     *         in 'message' or 'message' is just handshake from peer.
     */
    static std::vector<BittorrentMessage> getMessages(std::vector<std::byte> const& message);

private:
    /*!
     * @param message - raw message from peer.
     * @return true if message contains part of handshake message, false if not.
     */
    static bool hasBittorrentHandshakePart(std::vector<std::byte> const& message);
};
