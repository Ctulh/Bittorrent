#include <iostream>

#include "FileReader.hpp"
#include "BencodeParser.hpp"
#include "RequestSender.hpp"
#include "RequestBuilder.hpp"


int main() {
    FileReader fileReader("/home/ctuh/Downloads/test.torrent");
    BencodeParser parser(fileReader.getData());

    auto res  = parser.getList("files");
    for(auto el: res)
        std::cout << el << std::endl << std::endl;

}
