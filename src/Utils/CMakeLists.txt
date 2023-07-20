set(LIB_NAME Utils)

set(HEADERS_PATH ${PROJECT_SOURCE_DIR}/include/Utils)

set(HEADERS ${HEADERS_PATH}/StringMethods.hpp)
set(SOURCES StringMethods.cpp)

add_library(${LIB_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC ${HEADERS_PATH})