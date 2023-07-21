//
// Created by ctuh on 7/21/23.
//

#include "NoSuchFileException.hpp"

NoSuchFileException::NoSuchFileException(std::string const& path): m_path(path) {}

const char * NoSuchFileException::what() const noexcept {
    return m_path.c_str();
}