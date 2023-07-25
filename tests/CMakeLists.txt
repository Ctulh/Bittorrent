set(TEST_NAME ${PROJECT_NAME}.Tests)

set(UTILS_PATH ${PROJECT_SOURCE_DIR}/include/Utils)
set(BENCODE_PATH ${PROJECT_SOURCE_DIR}/include/Bencode)
set(LOGGER_PATH ${PROJECT_SOURCE_DIR}/include/Logger)

set(SOURCES main_test.cpp)
set(HEADERS StringOperationsTest.hpp
             BencodeParsingTests.hpp
             FileReaderTests.hpp
             LoggerTests.hpp)

set(GTEST_LIB CONAN_PKG::gtest)
set(GTEST_PATH ${CONAN_INCLUDE_DIRS_GTEST})

add_executable(${TEST_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${TEST_NAME} PUBLIC ${GTEST_PATH} ${UTILS_PATH} ${BENCODE_PATH} ${LOGGER_PATH})
target_link_libraries(${TEST_NAME} PRIVATE ${GTEST_LIB} BencodeParser Utils FileReader Logger)

add_custom_command(TARGET ${TEST_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/tests/test.txt ${PROJECT_BINARY_DIR}/bin/test.txt)