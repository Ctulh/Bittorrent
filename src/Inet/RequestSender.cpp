//
// Created by ctuh on 7/21/23.
//
#include "RequestSender.hpp"
#include <boost/asio/buffer.hpp>
#include <iostream>

std::string RequestSender::sendRequest(beast::http::request<http::string_body> const& request, std::string const& host, std::string const& port) {
    static asio::io_context ioContext;
    tcp::resolver resolver(ioContext);
    tcp::resolver::query query(host, port);
    auto const results = resolver.resolve(query);
    beast::tcp_stream stream(ioContext);
    stream.connect(results);
    http::write(stream, request);
    beast::flat_buffer buffer;

    http::response<http::dynamic_body> res;

    http::read(stream, buffer, res);

    std::cout << res << std::endl;

    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);
    return "";
}
