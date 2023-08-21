
#pragma once

#include <gtest/gtest.h>

#include "BencodeFile.hpp"

TEST(BencodeFileTest, readAnnounce) {
    const std::string torrentFilename = "test.torrent";
    const std::string expectedAnnounce = "http://some.pretty.url/ann?pk=1234";

    std::unique_ptr<BencodeFile> torrentFile;

    EXPECT_NO_THROW(torrentFile = std::make_unique<BencodeFile>(torrentFilename));
    std::string announce;
    EXPECT_NO_THROW(announce = torrentFile->getAnnounce());
    EXPECT_EQ(announce, expectedAnnounce);
}

TEST(BencodeFileTest, readTotalBytesLength) {
    const std::string torrentFilename = "test.torrent";
    const std::string expectedTotalBytesLeft = "7777";

    std::unique_ptr<BencodeFile> torrentFile;

    EXPECT_NO_THROW(torrentFile = std::make_unique<BencodeFile>(torrentFilename));
    std::string totalBytesLength;
    EXPECT_NO_THROW(totalBytesLength = torrentFile->getTotalBytesLeft());
    EXPECT_EQ(expectedTotalBytesLeft, totalBytesLength);
}

TEST(BencodeFileTest, readInfoHash) {
    const std::string torrentFilename = "test.torrent";
    const std::array<std::byte, 20> expectedHash {std::byte(0xAC), std::byte(0x51), std::byte(0x95), std::byte(0x67), std::byte(0x44),
                                                  std::byte(0xE7), std::byte(0xDA), std::byte(0x34), std::byte(0x3B), std::byte(0xCC),
                                                  std::byte(0xA4), std::byte(0x02), std::byte(0x07), std::byte(0x0D), std::byte(0x40),
                                                  std::byte(0x84), std::byte(0xDD), std::byte(0xBA), std::byte(0x44), std::byte(0x0F)};

    std::unique_ptr<BencodeFile> torrentFile;

    EXPECT_NO_THROW(torrentFile = std::make_unique<BencodeFile>(torrentFilename));
    TorrentHash infoHash;
    EXPECT_NO_THROW(infoHash = torrentFile->getInfoHash());
    EXPECT_EQ(infoHash, expectedHash);
}
