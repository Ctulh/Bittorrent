//
// Created by ctuh on 7/21/23.
//

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;


class RequestBuilder {
public:
    static http::request<http::string_body> createRequest(http::verb httpMethod, std::string const& host, std::string const& target, std::unordered_map<std::string, std::string> const& values);
};