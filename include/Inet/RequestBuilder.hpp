//
// Created by ctuh on 7/21/23.
//

#pragma once

#include <boost/beast/http.hpp>

#include "Bencode/BencodeFile.hpp"

using httpRequest = boost::beast::http::request<boost::beast::http::string_body>;

class RequestBuilder {
public:
    static httpRequest buildTrackerRequest(BencodeFile& torrentFile, std::string const& port); //TODO const
};
