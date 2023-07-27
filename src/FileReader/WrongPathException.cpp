//
// Created by ctuh on 7/21/23.
//

#include "WrongPathException.hpp"

WrongPathException::WrongPathException(std::string const& path): m_path(path) {}

const char * WrongPathException::what() const noexcept {
    return m_path.c_str();
}
