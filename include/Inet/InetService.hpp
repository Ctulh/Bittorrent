//
// Created by ctuh on 7/27/23.
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

class InetService {
private:
    InetService() = default;

public:
    static InetService& getInstance();
    std::string sendHttpRequest(beast::http::request<http::string_body> const& request, std::string const& host, const std::string &port = "80");
    boost::asio::io_context& getContext();

private:
    bool connect(tcp::socket& socket, std::string const& host, std::string const& port);
private:
    boost::asio::io_context m_context;
};