set(TEST_NAME ${PROJECT_NAME}.Tests)

set(SOURCES main_test.cpp)
set(HEADERS )

set(GTEST_LIB CONAN_PKG::gtest)
set(GTEST_PATH ${CONAN_INCLUDE_DIRS_GTEST})

add_executable(${TEST_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${TEST_NAME} PUBLIC ${GTEST_PATH} ${PROJECT_SOURCE_DIR}/include/Bencode)
target_link_libraries(${TEST_NAME} PRIVATE ${GTEST_LIB} BencodeParser)