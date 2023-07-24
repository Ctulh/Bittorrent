cmake_minimum_required(VERSION 3.25)
project(Bittorrent)

set(CMAKE_CXX_STANDARD 23)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

#add_compile_options(-Wextra -Wconversion -Wpedantic -Wall)

add_executable(Bittorrent app/main.cpp)

add_subdirectory(src)
add_subdirectory(tests)
target_link_libraries(Bittorrent Inet Utils BencodeParser FileReader)

