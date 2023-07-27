#include <iostream>

#include "FileReader.hpp"
#include "BencodeParser.hpp"
#include "InetService.hpp"
#include "RequestBuilder.hpp"
#include "InetService.hpp"
#include "Bittorrent/TorrentFile.hpp"
#include <algorithm>

int main() {
    TorrentFile torrentFile("/home/ctuh/Downloads/test.torrent");

    std::string host = "bt3.t-ru.org";
    auto request = RequestBuilder::buildTrackerRequest(torrentFile, host);


    std::cout << InetService::getInstance().sendHttpRequest(request, host, "80");
}
