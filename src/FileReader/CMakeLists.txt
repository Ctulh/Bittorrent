set(LIB_NAME FileReader)

set(HEADERS_PATH ${PROJECT_SOURCE_DIR}/include/FileReader)
set(UTILS_HEADERS_PATH ${PROJECT_SOURCE_DIR}/include/Utils)

set(SOURCES FileReader.cpp
            NoSuchFileException.cpp
            WrongPathException.cpp)

set(HEADERS ${HEADERS_PATH}/FileReader.hpp
            ${HEADERS_PATH}/NoSuchFileException.hpp
            ${HEADERS_PATH}/WrongPathException.hpp)

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${LIB_NAME} PUBLIC ${HEADERS_PATH} ${UTILS_HEADERS_PATH})
target_link_libraries(${LIB_NAME} PRIVATE Utils)
