set(LIB_NAME BencodeParser)

set(HEADERS_PATH ${PROJECT_SOURCE_DIR}/include/Bencode)

set(SOURCES BencodeParser.cpp)
set(HEADERS ${HEADERS_PATH}/BencodeParser.hpp)

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR})