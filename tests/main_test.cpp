//
// Created by ctuh on 7/20/23.
//

#include <gtest/gtest.h>

#include "BencodeParsingTests.hpp"
#include "UtilsTests/StringOperationsTest.hpp"
#include "UtilsTests/HashMethodsTest.hpp"
#include "UtilsTests/UrlMethodsTest.hpp"
#include "FileReaderTests.hpp"
#include "LoggerTests.hpp"
#include "BittorrentTests/TorrentFileTests.hpp"
#include "BittorrentTests/BittorrentMessageBuilderTests.hpp"

int main(int argc, char** argv) {
    using testing::InitGoogleTest;
    InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}