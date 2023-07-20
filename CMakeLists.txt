cmake_minimum_required(VERSION 3.25)
project(Bittorrent)

set(CMAKE_CXX_STANDARD 23)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(src)

add_executable(Bittorrent app/main.cpp include/TorrentFileParser.hpp src/TorrentFileParser.cpp tests/main_test.cpp tests/TorrentFileParserTests.hpp tests/BencodeParsingTests.hpp src/Bencode/BencodeParser.cpp include/Bencode/BencodeParser.hpp)

add_subdirectory(tests)
