#pragma once

#include <vector>
#include "Peer.hpp"
#include "TorrentFile.hpp"
#include "Inet/SocketPoller.hpp"
#include "Bittorrent/BittorrentMessages/BittorrentMessage.hpp"
#include <atomic>
#include <queue>
#include "TrackerResponse.hpp"
#include "Utils/Timer.hpp"
#include "MessageHandler.hpp"

struct Task {
    unsigned long piece;
    int begin;
};

class Torrent {
public:
    Torrent(std::string const& filepath);
    ~Torrent();
public:
    void run();
    void stop();
private:
    bool initPeers();
    bool initFiles();


private:

    TrackerResponse sendTrackerRequest() const;
    void addPeerIfConfirmHandshake(std::vector<Peer> const& peers, std::string const& handshakeMessage);
    std::string getInfoHash() const;
    std::string createHandshakeMessage() const;
    void produceTasks();
    void consumeTasks();
    void addFilePieceToQueue(TorrentFile const& file);

    void handle(std::string const& host, std::vector<std::byte> const& message);
    void chokeHandle(std::string const& host, std::vector<std::byte> const& payload);
    void unchokeHandle(std::string const& host, std::vector<std::byte> const& payload);
    void interestedHandle(std::string const& host, std::vector<std::byte> const& payload);
    void notInterestedHandle(std::string const& host, std::vector<std::byte> const& payload);
    void haveHandle(std::string const& host, std::vector<std::byte> const& payload);
    void bitfieldHandle(std::string const& host, std::vector<std::byte> const& payload);
    void requestHandle(std::string const& host, std::vector<std::byte> const& payload);
    void pieceHandle(std::string const& host, std::vector<std::byte> const& payload);
    void cancelHandle(std::string const& host, std::vector<std::byte> const& payload);
    void portHandle(std::string const& host, std::vector<std::byte> const& payload);


private:
    std::string m_torrentName;
    std::unique_ptr<MessageHandler> m_messageHandler;
    std::unique_ptr<std::thread> m_consumerThread;
    std::unique_ptr<std::thread> m_producerThread;
    std::unique_ptr<Timer> m_timer;
    std::queue<Task> m_taskQueue;
    std::atomic_flag m_isRunning;
    std::size_t m_pieceLength;
    std::string m_torrentFilePath;
    std::unordered_map<std::string, Peer> m_hostToPeer;
    std::unique_ptr<SocketPoller> m_poller;
    std::vector<TorrentFile> m_files;
};
