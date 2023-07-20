//
// Created by ctuh on 7/20/23.
//

#include "Exceptions/NoSuchFieldException.hpp"

NoSuchFieldException::NoSuchFieldException(const std::string &fieldName): m_fieldName(fieldName) {}

const char *NoSuchFieldException::what() const noexcept { return m_fieldName.c_str(); }
