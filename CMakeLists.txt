cmake_minimum_required(VERSION 3.25)
project(Bittorrent)
set(NAME app)

set(CMAKE_CXX_STANDARD 23)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

#add_compile_options(-Wextra -Wconversion -Wpedantic -Wall)

add_subdirectory(third_party/InetLib)

add_executable(${NAME} app/main.cpp)

add_subdirectory(src)
add_subdirectory(tests)
target_link_libraries(${NAME} Inet Utils BencodeParser FileReader Logger InetLib)

