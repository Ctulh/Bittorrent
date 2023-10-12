//
// Created by ctuh on 10/12/23.
//

#pragma once

#include <string>

class FileWriter {
public:
    FileWriter(std::string const& filepath);
    ~FileWriter();

public:
    void seek(std::size_t offset) const;
    void write(const void* buf, std::size_t size) const;
    void sync() const ;

private:
    int m_fd;
    std::string m_filepath;
};
