//
// Created by ctuh on 7/28/23.
//

#pragma once

#include <vector>
#include <string>

#include "Bencode/CanBeBuiltFromBencodeData.hpp"
#include "Logger/Logger.hpp"

class TrackerResponse: public CanBeBuiltFromBencodeData<TrackerResponse> {
public:
    TrackerResponse() = default;

public:
    virtual std::vector<std::pair<std::string, char>> getFieldsToFill() const override {
        return {{"interval", 'v'}, {"peers", 'v'}};
    }

    TrackerResponse(std::string const& data): CanBeBuiltFromBencodeData(data) {
        m_interval = m_bencodeValues["interval"];
        m_peers = m_bencodeValues["peers"];
    }

public:
    std::string m_interval;
    std::string m_peers;


    friend CanBeBuiltFromBencodeData<TrackerResponse>;
};