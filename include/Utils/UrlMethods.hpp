//
// Created by ctuh on 7/27/23.
//

#pragma once

#include <string>

namespace UrlMethods {
    std::string getDomainName(std::string const& url);
    std::string getPathToFile(std::string const& url);
    std::string getParameters(std::string const& url);
}