cmake_minimum_required(VERSION 3.25)
project(Bittorrent)

set(CMAKE_CXX_STANDARD 23)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(Bittorrent app/main.cpp include/Bencode/Exceptions/NoSuchFieldException.hpp include/Utils/StringMethods.hpp src/Utils/StringMethods.cpp)

add_subdirectory(src)
add_subdirectory(tests)
