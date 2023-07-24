//
// Created by ctuh on 7/20/23.
//

#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <vector>

using OptionalString = std::optional<std::string>;
using StringVector = std::vector<std::string>;


class BencodeParser {
public:
    BencodeParser(std::string const& data);

public:
    std::string operator[](std::string const& fieldName);
    std::string getValue(std::string const& fieldName);
    StringVector getList(std::string const& fieldName);

private:
    std::string readNumber(std::size_t& index) const;
    std::string readDictionary(std::size_t& index) const;
    std::string readString(std::size_t& index) const;
    StringVector readList(std::size_t& index) const;

private:
    std::unordered_map<std::string, OptionalString> m_fieldValues;
    std::string m_data;
    const char m_listSeparator = ',';
};