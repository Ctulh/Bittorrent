//
// Created by ctuh on 10/12/23.
//

#include "Utils/FileWriter.hpp"

#include <fcntl.h>
#include <unistd.h>

FileWriter::FileWriter(std::string const& filepath): m_fd(0), m_filepath(filepath) {
        int flags = O_WRONLY | O_CREAT;
        m_fd = open(filepath.c_str(), flags, 0666);
}

FileWriter::~FileWriter() {
    if(m_fd != 0) {
        close(m_fd);
    }
}

void FileWriter::seek(std::size_t offset) const {
    ::lseek(m_fd, offset, SEEK_SET);
}

void FileWriter::write(const void* buf, std::size_t size) const {
    ::write(m_fd, buf, size);
}

void FileWriter::sync() const {
    ::fsync(m_fd);
}