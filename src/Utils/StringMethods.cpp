#include "StringMethods.hpp"

#include <algorithm>
#include <ranges>

namespace StringMethods {
    std::string strip(std::string const &str) {
        std::input_iterator auto begin = std::ranges::find_if(str.begin(), str.end(), [](auto symbol) {
            return symbol != ' ';
        });

        std::input_iterator auto end = std::ranges::find_if(str.rbegin(), str.rend(), [](auto symbol) {
            return symbol != ' ';
        });

        return {begin, end.base()};
    }

    StringVector split(std::string const &str, char delimiter) {
        StringVector result;

        std::string buf;
        std::ranges::for_each(str.begin(), str.end(), [&result, &buf, delimiter](auto symbol) {
            if (symbol == delimiter) {
                if (!buf.empty()) {
                    result.push_back(buf);
                }
                buf.clear();
                return;
            }
            buf += symbol;
        });
        if (not buf.empty())
            result.push_back(buf);

        return result;
    }

    bool isStringANumber(const std::string &str) {
        bool isNumber = true;
        std::for_each(str.begin(), str.end(), [&isNumber](char el) {
            if (!std::isdigit(el)) {
                isNumber = false;
            }
        });
        return isNumber;
    }
}