//
// Created by ctuh on 7/21/23.
//
#include "RequestBuilder.hpp"

http::request<http::string_body> RequestBuilder::createRequest(http::verb httpMethod, std::string const& host, std::string const& target,
                                                               const std::unordered_map<std::string, std::string> &values) {
    const int httpVersion = 11;
    http::request<http::string_body> httpRequest{httpMethod, target, httpVersion};
    httpRequest.set(http::field::host, host);
    httpRequest.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    for(auto&& [fieldName, fieldValue]: values) {
        httpRequest.set(fieldName, fieldValue);
    }

    return httpRequest;
}
