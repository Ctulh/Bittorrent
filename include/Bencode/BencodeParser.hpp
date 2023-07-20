//
// Created by ctuh on 7/20/23.
//

#pragma once

#include <string>
#include <unordered_map>

class BencodeParser {
public:
    BencodeParser(std::string const& data);

public:
    std::string operator[](std::string const& fieldName);


private:
    std::unordered_map<std::string, std::string> m_fieldValues;
};