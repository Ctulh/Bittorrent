//
// Created by ctuh on 7/28/23.
//

#pragma once

#include <vector>
#include <string>
#include <cinttypes>

#include "Bencode/CanBeBuiltFromBencodeData.hpp"
#include "PeerInfo.hpp"
#include "Logger/Logger.hpp"
#include <boost/lexical_cast.hpp>

/*!
 * @brief Class represents http response from tracker. It contains peers addresses and update interval.
 *        You can learn more at the BitTorrent protocol specification
 */

class TrackerResponse: public CanBeBuiltFromBencodeData<TrackerResponse> {
public:
    TrackerResponse() = default;
    explicit TrackerResponse(std::string const& bencodeData);

public:
/*!
 * @return vector of fieldNames that expected to be in 'bencodeData'.
 *         'CanBeBuiltFromBencodeData' will look for this fields in 'bencodeData' and put values in map.
*/
    std::vector<std::pair<std::string, char>> getFieldsToFill() const override;

/*!
 * @return vector of peers.
*/
    std::vector<PeerInfo> getPeers();

public:
    std::uint32_t m_interval = 0;
    std::string m_peers;

    friend CanBeBuiltFromBencodeData<TrackerResponse>;
};
