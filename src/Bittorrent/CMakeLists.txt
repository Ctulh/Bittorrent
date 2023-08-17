set(LIB_NAME Bittorrent)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(HEADERS_PATH ${PROJECT_SOURCE_DIR}/include/Bittorrent)

set(HEADERS ${HEADERS_PATH}/TorrentFile.hpp
            ${HEADERS_PATH}/PeerInfo.hpp
            ${HEADERS_PATH}/Peer.hpp)

set(SOURCES TorrentFile.cpp
            PeerInfo.cpp
            Peer.cpp
        BittorrentMessages/BittorrentMessageBuilder.cpp)

add_library(${LIB_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC ${HEADERS_PATH} ${INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/third_party/InetLib/include/)
target_link_libraries(${LIB_NAME} PRIVATE BencodeParser FileReader Logger InetLib)
