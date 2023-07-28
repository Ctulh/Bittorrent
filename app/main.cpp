#include <iostream>

#include "FileReader.hpp"
#include "BencodeParser.hpp"
#include "InetService.hpp"
#include "RequestBuilder.hpp"
#include "InetService.hpp"
#include "Bittorrent/TorrentFile.hpp"
#include <algorithm>
#include <RequestSender.hpp>
#include "Utils/UrlMethods.hpp"
#include <Bittorrent/TrackerResponse.hpp>
#include "Logger/Logger.hpp"
#include "Logger/LoggerSpdlog.hpp"
#include "Utils/HashMethods.hpp"

#include <boost/asio.hpp>

void connect_handler(const boost::system::error_code& error)
{
    if (!error)
    {
        // Connect succeeded.
    }
}

int main() {
    Logger::addLogger("spdLogger", std::make_shared<LoggerSpdlog>("spdLogger", "logs.txt"));
    TorrentFile torrentFile("/home/ctuh/Downloads/test.torrent");

    std::string announce = torrentFile.getAnnounce();
    auto request = RequestBuilder::buildTrackerRequest(torrentFile, UrlMethods::getDomainName(announce));
    auto response = RequestSender::sendRequest<TrackerResponse>(request, UrlMethods::getDomainName(announce), "80");

    std::cout << response.m_interval << std::endl;
    std::cout << response.m_peers << std::endl;

    auto peers = response.m_peers;

    for(std::size_t i = 12; i < peers.size(); i += 6) {
        std::stringstream address;
        std::stringstream port;
        address << static_cast<int>(static_cast<uint8_t>(peers[i])) << '.';
        address << static_cast<int>(static_cast<uint8_t>(peers[i+1])) << '.';
        address << static_cast<int>(static_cast<uint8_t>(peers[i+2])) << '.';
        address << static_cast<int>(static_cast<uint8_t>(peers[i+3]));
        port << (static_cast<int>(static_cast<uint8_t>(peers[i+4])) | static_cast<int>(static_cast<uint8_t>(peers[i+5])) << 8);

        std::cout <<  address.str() << ':' << port.str() << std::endl;
    }
}
