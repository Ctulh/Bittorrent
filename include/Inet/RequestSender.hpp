//
// Created by ctuh on 7/28/23.
//

#pragma once

#include <concepts>
#include <string>
#include <boost/beast/http.hpp>

#include "Bencode/CanBeBuiltFromBencodeData.hpp"
#include "InetService.hpp"

namespace beast = boost::beast;



class RequestSender {
public:
    template<CanConstructedFromBencodeData ResponseType>
    static ResponseType sendRequest(beast::http::request<beast::http::string_body> const& request, std::string const& host, const std::string &port = "80") {
        ResponseType response(InetService::getInstance().sendHttpRequest(request, host, port));
        return response;
    }
};
