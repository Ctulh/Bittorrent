//
// Created by ctuh on 8/21/23.
//

#pragma once

#include <string>

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

enum class TorrentFileStatus {
    NotDownloaded = 0,
    PartiallyDownloaded = 1,
    Complete = 2,
    Paused = 3,
    Error = 4,
};

class TorrentFile {
public:
    TorrentFile(std::string const& filePath, std::size_t size, std::size_t fileOffset = 0): m_filePath(filePath),
                                                                m_totalSize(size),
                                                                m_fileStatus(TorrentFileStatus::NotDownloaded),
                                                                m_bytesDownloaded(0),
                                                                m_fileOffset(fileOffset) {
        m_fileStream.open(m_filePath, std::ios::binary | std::ios::out);
    }

    ~TorrentFile() {
        if(m_fileStream.is_open())
            m_fileStream.close();
    }

public:
    std::string getFilePath() const {
        return m_filePath;
    }

    std::size_t getSize() const {
        return m_totalSize;
    }

    TorrentFileStatus getStatus() const {
        if(m_fileStatus == TorrentFileStatus::NotDownloaded && m_bytesDownloaded > 0 && m_bytesDownloaded < m_totalSize)
            m_fileStatus = TorrentFileStatus::PartiallyDownloaded;
        else if(m_fileStatus == TorrentFileStatus::NotDownloaded && m_bytesDownloaded == m_totalSize)
            m_fileStatus = TorrentFileStatus::Complete;
        return m_fileStatus;
    }

    void writeData(std::vector<std::byte> data, std::size_t offset = 0) { // TODO realize offset
        m_fileStream.write(reinterpret_cast<char*>(data.data()), static_cast<long>(data.size()));
        m_bytesDownloaded += data.size();
    }

    std::size_t getBytesDownloaded() const {
        return m_bytesDownloaded;
    }

    std::size_t getBytesLeft() const {
        return m_totalSize - m_bytesDownloaded;
    }

    std::size_t getGlobalOffset() const {
        return m_fileOffset + m_bytesDownloaded;
    }

private:
    std::ofstream m_fileStream;
    mutable TorrentFileStatus m_fileStatus;
    std::size_t m_totalSize;
    std::size_t m_bytesDownloaded;
    std::size_t m_fileOffset;
    std::string m_filePath;
};


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

