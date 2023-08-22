set(LIB_NAME Utils)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(HEADERS_PATH ${PROJECT_SOURCE_DIR}/include/Utils)

set(HEADERS ${HEADERS_PATH}/StringMethods.hpp
            ${HEADERS_PATH}/HashMethods.hpp
            ${HEADERS_PATH}/UrlMethods.hpp
            ${HEADERS_PATH}/ByteMethods.hpp)

set(SOURCES StringMethods.cpp
            HashMethods.cpp
            UrlMethods.cpp
            ByteMethods.cpp)

add_library(${LIB_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC ${HEADERS_PATH} ${INCLUDE_DIR})