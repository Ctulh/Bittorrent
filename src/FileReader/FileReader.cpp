//
// Created by ctuh on 7/21/23.
//

#include "FileReader.hpp"
#include "StringMethods.hpp"
#include "WrongPathException.hpp"
#include "NoSuchFileException.hpp"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <regex>

FileReader::FileReader(std::string const& path): m_path(path) {
    StringVector pathComponents;
    pathComponents = StringMethods::split(path, std::filesystem::path::preferred_separator);
    const std::size_t componentsAmount = pathComponents.size();
    const std::regex windowsGlobalPathRegex (R"([a-zA-z])");

    std::string currPath;

    int i = 0;
    if(!m_path.empty() && m_path[0] == '/')
        currPath += '/';
    else if(!path.empty() && std::regex_match(pathComponents[0], windowsGlobalPathRegex)) {
        currPath = pathComponents[0];
        ++i;
    }

    for(; i < componentsAmount - 1; ++i) {
        if(!std::filesystem::exists(std::filesystem::path(currPath))) {
            throw WrongPathException(currPath);
        }
        currPath += pathComponents[i];
        currPath += std::filesystem::path::preferred_separator;
    }

    currPath+= pathComponents.back();
    if(!std::filesystem::exists(std::filesystem::path(currPath))) {
        throw NoSuchFileException(currPath);
    }
}

std::string FileReader::getData() const {
    std::ifstream file(m_path);
    std::string result((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return result;
}