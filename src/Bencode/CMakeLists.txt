set(LIB_NAME BencodeParser)

set(HEADERS_PATH ${PROJECT_SOURCE_DIR}/include/Bencode)

set(SOURCES BencodeParser.cpp
            BencodeFile.cpp
            Exceptions/NoSuchFieldException.cpp)

set(HEADERS ${HEADERS_PATH}/BencodeParser.hpp
            ${HEADERS_PATH}/BencodeFile.hpp
            ${HEADERS_PATH}/Exceptions/NoSuchFieldException.hpp)

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${LIB_NAME} PUBLIC ${HEADERS_PATH})
target_link_libraries(${LIB_NAME} PRIVATE Utils)