//
// Created by ctuh on 7/21/23.
//
#include "RequestSender.hpp"
#include <boost/asio/buffer.hpp>
#include <iostream>
#include <thread>

std::string RequestSender::sendRequest(beast::http::request<http::string_body> const& request, std::string const& host, std::string const& port) {
    static asio::io_context ioContext;

    tcp::resolver resolver(ioContext);
    tcp::resolver::query query(host, port);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
    tcp::socket socket(ioContext);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    if (error)
        throw boost::system::system_error(error);

    //beast::tcp_stream stream(ioContext);
    //stream.connect(results);
    http::write(socket, request);
    //http::write(stream, request);
    beast::flat_buffer buffer;

    http::response<http::dynamic_body> res;

    http::read(socket, buffer, res);

    std::stringstream ss;
    std::cout << res;
    ss << res;

    beast::error_code ec;
    socket.shutdown(tcp::socket::shutdown_both, ec);
    return "";
}
