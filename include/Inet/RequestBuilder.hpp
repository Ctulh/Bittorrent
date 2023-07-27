//
// Created by ctuh on 7/21/23.
//

#pragma once

#include <boost/beast/http.hpp>

#include "Bittorrent/TorrentFile.hpp"

using httpRequest = boost::beast::http::request<boost::beast::http::string_body>;

class RequestBuilder {
public:
    static httpRequest buildTrackerRequest(TorrentFile& torrentFile, std::string const& port); //TODO const
};
