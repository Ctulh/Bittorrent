#pragma once

#include <vector>
#include "PeerInfo.hpp"
#include "Peer.hpp"
#include "TorrentFile.hpp"
#include "Inet/SocketPoller.hpp"
#include "Bittorrent/BittorrentMessages/BittorrentMessage.hpp"

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
    void callback(std::vector<std::byte> const& message, StreamSocketPtr streamSocket);

    void haveHandle(BittorrentMessage const& message, StreamSocketPtr streamSocket);
    void bitfieldHandle(BittorrentMessage const& message, StreamSocketPtr streamSocket);

private:
    std::size_t m_pieceLength;
    bool m_isFileInited;
    std::string m_torrentFilePath;
    std::unordered_map<std::string, Peer> m_hostToPeer;
    std::unique_ptr<SocketPoller> m_poller;
    std::vector<TorrentFile> m_files;
    std::string m_handshake;
};
