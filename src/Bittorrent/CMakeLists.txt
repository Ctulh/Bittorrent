set(LIB_NAME Bittorrent)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(HEADERS_PATH ${PROJECT_SOURCE_DIR}/include/Bittorrent)

set(HEADERS ${HEADERS_PATH}/TorrentFile.hpp)
set(SOURCES TorrentFile.cpp)

add_library(${LIB_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC ${HEADERS_PATH} ${INCLUDE_DIR})
target_link_libraries(${LIB_NAME} PUBLIC BencodeParser FileReader)
