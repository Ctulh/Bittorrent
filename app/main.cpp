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

std::size_t connectedPeers = 0;
std::size_t totalPeers = 0;
#include "Bittorrent/BittorrentMessages/BittorrentMessageBuilder.hpp"
#include "Bittorrent/BittorrentMessages/Interested.hpp"
#include "Bittorrent/BittorrentMessages/Request.hpp"
#include "Bittorrent/BittorrentMessages/BittorrentMessageParser.hpp"
#include "Utils/ByteMethods.hpp"

int main() {

    boost::asio::io_context context;
    Logger::addLogger("spdLogger", std::make_shared<LoggerSpdlog>("spdLogger", "logs.txt"));
    BencodeFile torrentFile("/home/ctuh/Downloads/3.torrent");

    auto files = torrentFile.getFiles();
    for(auto file: files) {
        std::cout << file << std::endl;
    }

    std::string announce = torrentFile.getAnnounce();
    auto request = RequestBuilder::buildTrackerRequest(torrentFile, UrlMethods::getDomainName(announce));
    auto response = RequestSender::sendRequest<TrackerResponse>(request, UrlMethods::getDomainName(announce), "80");

   // std::cout << response.m_interval << std::endl;
    //std::cout << response.m_peers << std::endl;



    std::string reqStr;
    reqStr.push_back(0x13);
    reqStr += "BitTorrent protocol";
    for(int i =0 ; i< 8;++i) {
        reqStr.push_back(0x0);
    }
    auto torrentHash =  torrentFile.getInfoHash();
    for(auto hashEl: torrentHash) {
        reqStr.push_back(static_cast<char>(hashEl));
    }

    reqStr += "-TR2940-k8hj0wgej6ch";


    auto peerInfos = response.getPeers();
    totalPeers = peerInfos.size();

    std::vector<std::shared_ptr<Peer>> peers;
    for(auto& peerInfo: peerInfos) {
        peers.push_back(std::make_shared<Peer>(peerInfo));
    }

    SocketPoller poller(10, 10000);

    auto callback = [](std::vector<std::byte> const& message, StreamSocketPtr streamSocket) {
        auto inetAddr = streamSocket->getInetAddress();
        try {
            Logger::logInfo(std::format("Got message on {}:{}", inetAddr.getHost(), inetAddr.getPort()));
            auto messages = BittorrentMessageParser::getMessages(message);
            for(auto message: messages) {
                std::cout << message << std::endl;
            }
        }
        catch(std::exception& e) {
            std::cout << e.what();
        }
    };

    poller.setReceiveMessageCallback(callback);

    std::thread t1([&poller]() {
        while(true) {
            poller.poll();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    for(auto& peer: peers) {
        if(peer->handshake(reqStr)) {
            auto socket = peer->getSocket();
            Interested interested;
            socket->send(interested.getMessage());
            poller.add(socket);
        }
    }

    t1.join();
}

