#include "UrlMethods.hpp"

#include <string>
#include <algorithm>
#include <numeric>

std::string UrlMethods::getDomainName(std::string const& url) {
    const std::string httpScheme = "http://";
    std::size_t index = 0;
    if(url.starts_with(httpScheme)) {
        index += httpScheme.size();
    }

    const std::size_t strLength = url.size();
    for(std::size_t i = index; i < strLength; ++i) {
        if(url[i] == '/' || url[i] == ':')
            return url.substr(index, i - index);
    }
    return url.substr(index);
}

std::string UrlMethods::getParameters(const std::string &url) {
    return "";
}

std::string UrlMethods::getPathToFile(const std::string &url) {
    const std::string httpScheme = "http://";
    std::size_t index = 0;
    if(url.starts_with(httpScheme)) {
        index += httpScheme.size();
    }
    auto slash = url.find('/', index);
    if(slash == url.npos)
        return "";

    index = slash;

    const std::size_t strLength = url.size();
    for(std::size_t i = index; i < strLength; ++i) {
        if(url[i] == '?' || url[i] == '#')
            return url.substr(index, i - index);
    }

    return url.substr(index);
}