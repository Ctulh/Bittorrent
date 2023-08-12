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
#include <atomic>
#include <mutex>
#include <shared_mutex>

#include "Bittorrent/Peer.hpp"

std::size_t connectedPeers = 0;
std::size_t totalPeers = 0;
#include "Bittorrent/BittorrentMessageBuilder.hpp"


class PeerWriter {
public:
    ~PeerWriter() {
        m_socket->close();
    }
    PeerWriter() = delete;
    PeerWriter(PeerInfo const& peer, boost::asio::io_context& context, std::string const& request): m_peer(peer), m_request(request) {
        m_response.resize(1024);
        boost::asio::ip::tcp::endpoint end(asio::ip::address::from_string(m_peer.getAddress()), m_peer.getPort());
        m_socket = std::make_shared<boost::asio::ip::tcp::socket>(context, end.protocol());
    }
     void run() {
         boost::asio::ip::tcp::endpoint end(asio::ip::address::from_string(m_peer.getAddress()), m_peer.getPort());
         m_socket->async_connect(end, [this](boost::system::error_code const& err) {
             this->handleConnect(err);
         });
    }
    void handleConnect(boost::system::error_code const& err) {
        if (!err)
        {
           // increaseConnectedPeers();
            Logger::logInfo(
                    std::format("Successful connection to {}:{}", m_peer.getAddress(),
                                m_peer.getPort()));
            boost::asio::async_write(*(m_socket), boost::asio::buffer(m_request, m_request.size()), [this](boost::system::error_code const& err, std::size_t transfered){
                this->handleWriteRequest(err, transfered);
            });
        }
        else {
            Logger::logInfo(std::format("Error connecting {}", err.message()));
        }
    }

    void handleWriteRequest(boost::system::error_code const& err, std::size_t transfered) {
        if (!err)
        {
            boost::asio::async_read(*(m_socket), boost::asio::buffer(m_response.data(), 1024),  [this](boost::system::error_code const& err, std::size_t recieved){
                this->handleRead(err, recieved);
            });
        }
        else
        {
            std::cout << "Error Write: " << std::endl;
        }
    }

    void handleRead(boost::system::error_code const& err, std::size_t recv) {
        if(!err) {
            Logger::logInfo(std::format("Successful read response: {}", std::string(m_response.data(), recv)));
        }
        else {
            std::cout << "Error Read: " << std::endl;
        }
    }
private:
    std::string m_response;
    const PeerInfo m_peer;
    std::shared_ptr<boost::asio::ip::tcp::socket> m_socket;
    std::string m_request;
};




int main() {
    boost::asio::io_context context;
    Logger::addLogger("spdLogger", std::make_shared<LoggerSpdlog>("spdLogger", "logs.txt"));
    TorrentFile torrentFile("/home/ctuh/Downloads/test2.torrent");

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
            BittorrentMessageBuilder builder;
            builder.setMessageType(MessageType::INTERESTED);
            socket.setSendTimeout(std::chrono::milliseconds(10000));
            socket.setReceiveTimeout(std::chrono::milliseconds(10000));
            socket.send(builder.getMessage());
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

