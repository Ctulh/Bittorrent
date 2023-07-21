//
// Created by ctuh on 7/21/23.
//

#pragma once

#include <exception>
#include <string>

class WrongPathException: std::exception {
public:
    WrongPathException(std::string const& path);

public:
    const char * what() const noexcept override;

private:
    std::string m_path;
};