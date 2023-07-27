#include <iostream>

#include "FileReader.hpp"
#include "BencodeParser.hpp"
#include "RequestSender.hpp"
#include "RequestBuilder.hpp"
#include "Bittorrent/TorrentFile.hpp"
#include <algorithm>

int main() {
    TorrentFile torrentFile("/home/ctuh/Downloads/test.torrent");

    std::string host = "bt3.t-ru.org";
    auto request = RequestBuilder::createTrackerRequest(torrentFile, host);


    std::cout << RequestSender::sendRequest(request, host, "80");
}
