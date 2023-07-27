//
// Created by ctuh on 7/27/23.
//
#include "InetService.hpp"
#include "Logger/Logger.hpp"

InetService &InetService::getInstance() {
    static InetService inetServiceInstance;
    return inetServiceInstance;
}

std::string InetService::sendHttpRequest(beast::http::request<http::string_body> const& request, std::string const& host, std::string const& port) {
    tcp::socket socket(m_context);
    if(!connect(socket, host, port)) {
        return "";
    }

    http::write(socket, request);
    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(socket, buffer, res);
    socket.shutdown(tcp::socket::shutdown_both);

    std::stringstream response;
    response << res;
    return response.str();
}

bool InetService::connect(tcp::socket &socket, const std::string &host, const std::string &port) {
    tcp::resolver resolver(m_context);
    tcp::resolver::query query(host, port);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    if(error) {
        Logger::logError(std::format("Failed connect to {}:{}. Error message: {}", host, port, error.message()));
        return false;
    }
    return true;
}
