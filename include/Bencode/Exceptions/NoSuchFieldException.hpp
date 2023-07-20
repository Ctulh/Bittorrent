//
// Created by ctuh on 7/20/23.
//

#pragma once

#include <exception>
#include <string>

class NoSuchFieldException: public std::exception {
public:
    NoSuchFieldException(std::string const& fieldName);

public:
    const char * what() const noexcept override;

private:
    std::string m_fieldName;
};