//
// Created by ctuh on 8/21/23.
//

#pragma once

#include <string>

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include <Bittorrent/TorrentFile.hpp>


TEST(TorrentFileTest, ConstructTorrentFile) {
    TorrentFile torrentFile("someFile.txt", 10, 0, 5);
}

TEST(TorrentFileTest, GetPieces) {
    std::string const filename = "somefile.txt";
    std::size_t const fileSize = 10;
    std::size_t const pieceOffset = 0;
    std::size_t const pieceSize = 5;

    TorrentFile torrentFile(filename, fileSize, pieceOffset, pieceSize);

    std::vector<BitTorrentPiece> pieces;
    EXPECT_NO_THROW(pieces = torrentFile.getPieces());
    EXPECT_EQ(pieces.size(), fileSize / pieceSize);
}
