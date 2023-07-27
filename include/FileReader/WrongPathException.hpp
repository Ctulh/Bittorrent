//
// Created by ctuh on 7/21/23.
//

#pragma once

#include <exception>
#include <string>

/**
 * @brief An exception that indicates an error in the file path.
 */

class WrongPathException: std::exception {
public:
    WrongPathException(std::string const& path);

public:
    const char * what() const noexcept override;

private:
    std::string m_path;
};