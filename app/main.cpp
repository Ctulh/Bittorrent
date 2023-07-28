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

int main() {
    Logger::addLogger("spdLogger", std::make_shared<LoggerSpdlog>("spdLogger", "logs.txt"));
    TorrentFile torrentFile("/home/ctuh/Downloads/test.torrent");

    std::string announce = torrentFile.getAnnounce();
    auto request = RequestBuilder::buildTrackerRequest(torrentFile, UrlMethods::getDomainName(announce));
    auto response = RequestSender::sendRequest<TrackerResponse>(request, UrlMethods::getDomainName(announce), "80");

    std::cout << response.m_interval << std::endl;
    std::cout << response.m_peers << std::endl;
}
