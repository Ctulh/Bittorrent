//
// Created by ctuh on 8/21/23.
//

#pragma once

#include <string>

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include <Bittorrent/TorrentFile.hpp>

class TorrentFileTestFixture: public testing::Test {
public:
    TorrentFileTestFixture(): torrentFile(filePath, fileSize) {}

    void SetUp() override {
        std::filesystem::path file(filePath);
        if(std::filesystem::exists(file)) {
            std::fstream fileStream(filePath, std::ios::out | std::ios::trunc);
            fileStream.close();
        }
    }
    void TearDown() override {

    }
public:
    const std::string filePath = "someFile.txt";
    const std::size_t fileSize = 10;
    TorrentFile torrentFile;
};

TEST(TorrentFileTest, constructTorrentFile) {
    const std::string filePath = "someFile.txt";
    const std::size_t fileSize = 1;
    std::unique_ptr<TorrentFile> torrentFile;

    EXPECT_NO_THROW(torrentFile = std::make_unique<TorrentFile>(filePath, fileSize));
    EXPECT_TRUE(torrentFile);
}

TEST_F(TorrentFileTestFixture, getFilePath) {
    EXPECT_EQ(torrentFile.getFilePath(), filePath);
}

TEST_F(TorrentFileTestFixture, getFileSize) {
    EXPECT_EQ(torrentFile.getSize(), fileSize);
}

TEST_F(TorrentFileTestFixture, getStatus) {
    const TorrentFileStatus expectedStatus = TorrentFileStatus::NotDownloaded;

    EXPECT_EQ(torrentFile.getStatus(), expectedStatus);
}

TEST_F(TorrentFileTestFixture, getBytesLeft) {
    EXPECT_EQ(torrentFile.getBytesLeft(), fileSize);
}

TEST_F(TorrentFileTestFixture, getBytesDownloaded) {
    EXPECT_EQ(torrentFile.getBytesDownloaded(), 0);
}

TEST_F(TorrentFileTestFixture, writeDataToTorrentFile) {
    const std::vector<std::byte> dataToWrite {std::byte(0x0)};

    EXPECT_NO_THROW(torrentFile.writeData(dataToWrite));
}

TEST_F(TorrentFileTestFixture, getBytesLeftAfterWriteData) {
    const std::vector<std::byte> dataToWrite {std::byte(0x0), std::byte(0x1)};
    const std::size_t expectedBytesLeft = fileSize - dataToWrite.size();

    EXPECT_NO_THROW(torrentFile.writeData(dataToWrite));
    EXPECT_EQ(torrentFile.getBytesLeft(), expectedBytesLeft);
}

TEST_F(TorrentFileTestFixture, getBytesDownloadedAfterWriteData) {
    const std::vector<std::byte> dataToWrite {std::byte(0x0), std::byte(0x1)};
    const std::size_t expectedBytesDownloaded = dataToWrite.size();

    EXPECT_NO_THROW(torrentFile.writeData(dataToWrite));
    EXPECT_EQ(torrentFile.getBytesDownloaded(), expectedBytesDownloaded);
}

TEST_F(TorrentFileTestFixture, getStatusAfterWriteSomeData) {
    const std::vector<std::byte> dataToWrite {std::byte(0x0), std::byte(0x1)};
    const TorrentFileStatus expectedStatus = TorrentFileStatus::PartiallyDownloaded;

    EXPECT_NO_THROW(torrentFile.writeData(dataToWrite));
    EXPECT_EQ(torrentFile.getStatus(), expectedStatus);
}

TEST_F(TorrentFileTestFixture, getStatusAfterAllData) {
    const std::vector<std::byte> dataToWrite {std::byte(0x0), std::byte(0x1), std::byte(0x2), std::byte(0x3), std::byte(0x4),
                                              std::byte(0x5), std::byte(0x6), std::byte(0x7), std::byte(0x8), std::byte(0x9)};
    const TorrentFileStatus expectedStatus = TorrentFileStatus::Complete;

    EXPECT_NO_THROW(torrentFile.writeData(dataToWrite));
    EXPECT_EQ(torrentFile.getStatus(), expectedStatus);
}

TEST_F(TorrentFileTestFixture, getGlobalOffsetByDefault) {
    const std::size_t expectedOffset = 0;

    EXPECT_EQ(torrentFile.getGlobalOffset(), expectedOffset);
}

TEST_F(TorrentFileTestFixture, getGlobalOffsetAfterWrite) {
    const std::vector<std::byte> dataToWrite {std::byte(0x0), std::byte(0x1)};
    const std::size_t expectedOffset = dataToWrite.size();

    EXPECT_NO_THROW(torrentFile.writeData(dataToWrite));
    EXPECT_EQ(torrentFile.getGlobalOffset(), expectedOffset);
}

TEST(TorrentTest, getGlobalOffsetWithValueInConstructor) {
    const std::string filePath = "someFile.txt";
    const std::size_t fileSize = 10;
    const std::size_t offset = 2;
    TorrentFile torrentFile(filePath, fileSize, offset);

    EXPECT_EQ(torrentFile.getGlobalOffset(), offset);
}

TEST(TorrentTest, getGlobalOffsetWithValueInConstructorAfterWrite) {
    const std::string filePath = "someFile.txt";
    const std::size_t fileSize = 10;
    const std::size_t offset = 2;
    const std::vector<std::byte> dataToWrite {std::byte(0x0), std::byte(0x1)};
    const std::size_t expectedOffset = offset + dataToWrite.size();
    TorrentFile torrentFile(filePath, fileSize, offset);

    EXPECT_NO_THROW(torrentFile.writeData(dataToWrite));
    EXPECT_EQ(torrentFile.getGlobalOffset(), expectedOffset);
}

