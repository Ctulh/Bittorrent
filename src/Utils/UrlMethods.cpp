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

std::string UrlMethods::getParameters(std::string const& url) {
    std::size_t index = url.find('?');
    if(index == url.npos)
        return "";

    const std::size_t strSize = url.size();
    for(std::size_t i = index; i < strSize; ++i) {
        if(url[i] == '#')
            return url.substr(index, i - index);
    }
    return url.substr(index);
}

std::string UrlMethods::getPathToFile(std::string const& url) {
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

std::string UrlMethods::getDataAfterDomainName(std::string const& url) {
    const std::string httpScheme = "http://";
    std::size_t index = 0;
    if(url.starts_with(httpScheme)) {
        index += httpScheme.size();
    }
    auto slash = url.find('/', index);
    if(slash == url.npos)
        return "";

    index = slash;
    return url.substr(index);
};
