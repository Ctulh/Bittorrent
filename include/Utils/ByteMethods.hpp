//
// Created by ctuh on 8/22/23.
//

#pragma once

#include <vector>
#include <cinttypes>

namespace ByteMethods {
    std::vector<std::byte> convertNumberToFourBytes(uint32_t number);
    std::size_t convertFourBytesToNumber(std::vector<std::byte> const& bytes);

}