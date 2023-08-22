//
// Created by ctuh on 8/22/23.
//

#include "ByteMethods.hpp"

#include <cmath>

std::vector<std::byte> ByteMethods::convertNumberToFourBytes(uint32_t number) {
    std::vector<std::byte> output(4);

    output[0] = static_cast<std::byte>(number >> 24);
    output[1] = static_cast<std::byte>(number >> 16);
    output[2] = static_cast<std::byte>(number >> 8);
    output[3] = static_cast<std::byte>(number);

    return output;
}

std::size_t ByteMethods::convertFourBytesToNumber(const std::vector<std::byte> &bytes) {
    if(bytes.size() < 4)
        return 0;

    std::size_t number = static_cast<std::size_t>(
                                        static_cast<unsigned int>(bytes[0]) << 24 |
                                        static_cast<unsigned int>(bytes[1]) << 16 |
                                        static_cast<unsigned int>(bytes[2]) << 8 |
                                        static_cast<unsigned int>(bytes[3])
                                     );
    return number;
}
