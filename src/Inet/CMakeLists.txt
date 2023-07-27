set(LIB_NAME Inet)

set(BOOST_LIB ${CONAN_PKG_LIBS_BOOST})
set(BOOST_INCLUDE_PATH ${CONAN_INCLUDE_DIRS_BOOST})
set(INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include)

set(HEADERS_PATH ${PROJECT_SOURCE_DIR}/include/Inet)

set(SOURCES RequestBuilder.cpp
            InetService.cpp)
set(HEADERS ${HEADERS_PATH}/RequestBuilder.hpp
            ${HEADERS_PATH}/InetService.hpp)

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${LIB_NAME} PUBLIC ${HEADERS_PATH} ${BOOST_INCLUDE_PATH} ${INCLUDE_PATH})
target_link_libraries(${LIB_NAME} PRIVATE CONAN_PKG::boost Bittorrent)
