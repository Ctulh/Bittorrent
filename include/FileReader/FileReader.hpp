//
// Created by ctuh on 7/21/23.
//

#pragma once

#include <string>

class FileReader {
public:
    FileReader(std::string const& path);

public:
    std::string getData() const;

private:
    std::string m_path;
};