set(LIB_NAME Logger)

set(HEADERS_PATH ${PROJECT_SOURCE_DIR}/include/Logger)

set(HEADERS ${HEADERS_PATH}/ILogger.hpp
            ${HEADERS_PATH}/LoggerSpdlog.hpp
            ${HEADERS_PATH}/Logger.hpp)

set(SOURCES LoggerSpdlog.cpp
            Logger.cpp)

add_library(${LIB_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC ${HEADERS_PATH} ${CONAN_INCLUDE_DIRS_SPDLOG})
