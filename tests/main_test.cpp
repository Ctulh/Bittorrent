//
// Created by ctuh on 7/20/23.
//

#include <gtest/gtest.h>

#include "BencodeParsingTests.hpp"
#include "StringOperationsTest.hpp"
#include "FileReaderTests.hpp"
#include "LoggerTests.hpp"
#include "BittorrentTests/TorrentFileTests.hpp"

int main(int argc, char** argv) {
    using testing::InitGoogleTest;
    InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}