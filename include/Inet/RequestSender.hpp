//
// Created by ctuh on 7/21/23.
//

#pragma once

#include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class RequestSender {
public:
    static std::string sendRequest(beast::http::request<http::string_body> const& request, std::string const& host, const std::string &port = "80");
};