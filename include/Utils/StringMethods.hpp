
#pragma once

#include <string>
#include <vector>

using StringVector = std::vector<std::string>;

namespace StringMethods {
    StringVector split(std::string const& str, char delimiter);
    std::string strip(std::string const& str);
    bool isStringANumber(std::string const& str);
}
