//
// Created by ctuh on 9/2/23.
//

#pragma once

#include <vector>

class BitfieldReader {
public:
    static std::vector<std::size_t> getPiecesFromBitfield(std::vector<std::byte> const& payload);

private:
    static bool getBit(std::byte byte, std::size_t position);
};
