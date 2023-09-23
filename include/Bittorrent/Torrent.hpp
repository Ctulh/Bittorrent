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

struct Task {
    unsigned long block;
    int begin;
};

class Torrent {
public:
    Torrent(std::string const& filepath);
public:
    void run();
    void stop();
private:
    bool initPeers();
    bool initFiles();


private:
    void handle(std::vector<std::byte> const& message, StreamSocketPtr streamSocket);

    //  void keepAliveHandle(BittorrentMessage const& message, StreamSocketPtr streamSocket);
//    void chokeHandle(BittorrentMessage const& message, StreamSocketPtr streamSocket);
//    void unchokeHandle(BittorrentMessage const& message, StreamSocketPtr streamSocket);
//    void interestedHandle(BittorrentMessage const& message, StreamSocketPtr streamSocket);
//    void notInterestedHandle(BittorrentMessage const& message, StreamSocketPtr streamSocket);
    void haveHandle(BittorrentMessage const& message, StreamSocketPtr streamSocket);
    void bitfieldHandle(BittorrentMessage const& message, StreamSocketPtr streamSocket);
    //  void requestHandle(BittorrentMessage const& message, StreamSocketPtr streamSocket);
    void pieceHandle(BittorrentMessage const& message, StreamSocketPtr streamSocket);
    TrackerResponse sendTrackerRequest() const;
    void addPeerIfConfirmHandshake(std::vector<Peer> const& peers, std::string const& handshakeMessage);
    std::string getInfoHash() const;
    std::string createHandshakeMessage() const;
    void setTimerTimeout(std::uint32_t timeout);
    void produceTasks();

private:
    std::unique_ptr<Timer> m_timer;
    std::queue<Task> m_taskQueue;
    std::atomic_flag m_isRunning;
    std::size_t m_pieceLength;
    bool m_isFileInited;
    std::string m_torrentFilePath;
    std::unordered_map<std::string, Peer> m_hostToPeer;
    std::unique_ptr<SocketPoller> m_poller;
    std::vector<TorrentFile> m_files;
};
