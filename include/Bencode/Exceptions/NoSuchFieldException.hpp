
#pragma once

#include <exception>
#include <string>

/**
 * @brief An exception that means the absence of a field in a bencode string. Can be thrown from BencodeParser.
 */

class NoSuchFieldException: public std::exception {
public:
    NoSuchFieldException(std::string const& fieldName);

public:
    const char * what() const noexcept override;

private:
    std::string m_fieldName;
};