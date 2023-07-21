//
// Created by ctuh on 7/21/23.
//

#pragma once

#include <gtest/gtest.h>
#include "FileReader.hpp"
#include "NoSuchFileException.hpp"
#include "WrongPathException.hpp"

TEST(FileReaderTest, TestReadFile) {
    const std::string pathToFile = "test.txt";

    FileReader fileReader(pathToFile);

    const std::string text = "text1337";
    std::string result;

    ASSERT_NO_THROW(result = fileReader.getData());
    ASSERT_EQ(result, text);
}

TEST(FileReaderTest, TestReadUnexistFile) {
    const std::string wrongFilename = "wrongFile.txt";

    ASSERT_THROW(FileReader reader(wrongFilename), NoSuchFileException);
}

TEST(FileReaderTest, TestReadFileInUnexistDirectory) {
    const std::string wrongFilepath = "wrongDirectory/test.txt";

    ASSERT_THROW(FileReader reader(wrongFilepath), WrongPathException);
}