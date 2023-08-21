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

#include <boost/asio.hpp>
#include <atomic>
#include <mutex>
#include <shared_mutex>

#include "Bittorrent/Peer.hpp"

std::size_t connectedPeers = 0;
std::size_t totalPeers = 0;
#include "Bittorrent/BittorrentMessages/BittorrentMessageBuilder.hpp"
#include "Bittorrent/BittorrentMessages/Interested.hpp"
#include "Bittorrent/BittorrentMessages/Request.hpp"


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

    for(auto& peer: peers) {
        if(peer->handshake(reqStr)) {
            auto socket = peer->getSocket();
            Interested interested;
            socket.setSendTimeout(std::chrono::milliseconds(10000));
            socket.setReceiveTimeout(std::chrono::milliseconds(10000));
            socket.send(interested.getMessage());
            std::string response;
            socket.receive(response);
            Logger::logInfo(response);
        }
    }


    context.run();

    for(auto& peer: peers) {
        std::cout << peer.get() << std::endl;
    }


    //Peer peer(context, peerInfo);
    //std::cout << peer.getResponse(requestStr) << std::endl;


    //std::vector<PeerWriter> peersVec;

  //  for(const auto& peer: peers) {
  //      auto fut = std::async(std::launch::async, [&peersVec, &peer, &reqStr, &context]() {
   //         PeerWriter writer(peer, context, reqStr);
  // //         writer.run();
  //          peersVec.push_back(std::move(writer));
  //      });
   // }

  //  std::this_thread::sleep_for(std::chrono::seconds(30));
   // std::this_thread::sleep_for(std::chrono::seconds(60));

 //   std::thread t([&]() {
 //      context.run();
 //   });
 //   t.join();
}

