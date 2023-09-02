//
// Created by ctuh on 9/2/23.
//

#include "Bittorrent/BittorrentMessages/BitfieldReader.hpp"

std::vector<std::size_t> BitfieldReader::getPiecesFromBitfield(std::vector<std::byte> const& payload) {
    std::vector<std::size_t> pieces;
    const std::size_t n = payload.size();
// TODO think about translate endian from network order
    for(std::size_t i = 0; i < n; ++i) {
        for(int bytePos = 7; bytePos >= 0; --bytePos) {
            if(getBit(payload[i], bytePos)) {
                pieces.push_back(i * 8 + (7-bytePos)); // (7-bytePos) because according to the documentation "The high bit in the first byte corresponds to piece index 0" may be will be changed to most beautiful way
            }
        }
    }
    return pieces;
}

bool BitfieldReader::getBit(std::byte byte, std::size_t position) {
    return (static_cast<unsigned char>(byte) >> position) & 0x1;
}
