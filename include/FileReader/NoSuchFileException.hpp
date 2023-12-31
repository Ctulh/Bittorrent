//
// Created by ctuh on 7/21/23.
//

#pragma once

#include <exception>
#include <string>

/**
 * @brief An exception that indicates the absence of a file.
 */

class NoSuchFileException: public std::exception {
public:
    NoSuchFileException(std::string const& path);

public:
    const char * what() const noexcept override;

private:
    std::string m_path;
};
