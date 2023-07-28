//
// Created by ctuh on 7/21/23.
//
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "RequestBuilder.hpp"
#include "Utils/HashMethods.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

httpRequest RequestBuilder::buildTrackerRequest(TorrentFile &torrentFile, std::string const& host) {
    const int httpVersion = 11;

    std::stringstream ss;
    std::string torrentAnnounce = torrentFile.getAnnounce();
    auto it = std::find(torrentAnnounce.rbegin(), torrentAnnounce.rend(), '/');
    //TODO exception if no backslash
    ss << torrentAnnounce.substr(it.base() - torrentAnnounce.begin()-1);
    ss << "&" << "info_hash=" << hash::urlEncodeHash(torrentFile.getInfoHash()) << "&"
       //<< "peer_id=" << hash::urlEncodeHash(hash::sha1("ABCDEFGHIJKLMNOPQRST")) << "&"
       << "peer_id=%EBk%A5%8D%97nz%0E%C2%F0X%FF%DD%B0%C7%99%D0%FE%81g" << "&"
       << "uploaded=" << "0" << "&"
       << "downloaded=" << "0" << "&"
       << "port=" << "6881" << "&"
       << "left=" << torrentFile.getTotalBytesLeft() << "&"
       << "event=started";

    std::string target = ss.str();

    http::request<http::string_body> httpRequest{http::verb::get, target, httpVersion};
    httpRequest.set(http::field::host, host);
    httpRequest.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    return httpRequest;
}
