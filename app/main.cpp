#include <iostream>

#include "FileReader.hpp"
#include "BencodeParser.hpp"
#include "InetService.hpp"
#include "RequestBuilder.hpp"
#include "InetService.hpp"
#include "Bencode/BencodeFile.hpp"
#include <algorithm>
#include <RequestSender.hpp>
#include "Utils/UrlMethods.hpp"
#include <Bittorrent/TrackerResponse.hpp>
#include "Logger/Logger.hpp"
#include "Logger/LoggerSpdlog.hpp"
#include "Utils/HashMethods.hpp"
#include "Inet/SocketPoller.hpp"


#include <boost/asio.hpp>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <chrono>

#include "Bittorrent/Peer.hpp"
#include "Bittorrent/Torrent.hpp"

std::size_t connectedPeers = 0;
std::size_t totalPeers = 0;
#include "Bittorrent/BittorrentMessages/BittorrentMessageBuilder.hpp"
#include "Bittorrent/BittorrentMessages/Interested.hpp"
#include "Bittorrent/BittorrentMessages/Request.hpp"
#include "Bittorrent/BittorrentMessages/HandshakeMessageBuilder.hpp"
#include "Bittorrent/BittorrentMessages/BittorrentMessageParser.hpp"
#include "Utils/ByteMethods.hpp"
#include "Bittorrent/BitTorrentIdentity.hpp"

int main() {
    Logger::addLogger("spdLogger", std::make_shared<LoggerSpdlog>("spdLogger", "logs.txt"));
    Torrent torrent("/home/ctuh/Downloads/test2.torrent");
    torrent.run();
}

