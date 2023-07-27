#pragma once

#include <string>

/**
 * @brief Class that provides access to file data.
 */
class FileReader {
public:
    FileReader(std::string const& path);

public:
    std::string getData() const;

private:
    std::string m_path;
};