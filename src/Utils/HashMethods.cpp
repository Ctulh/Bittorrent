//
// Created by ctuh on 7/26/23.
//

#include "HashMethods.hpp"

#include <boost/uuid/detail/sha1.hpp>
#include <sstream>

using namespace hash;

Sha1Hash hash::stringHashToBytes(const std::string &hash) {
    Sha1Hash bytes;

    std::size_t counter = 0;
    for (unsigned int i = 0; i < hash.length(); i += 2) {
        std::string byteString = hash.substr(i, 2);
        auto byte = static_cast<std::byte>(strtol(byteString.c_str(), NULL, 16));
        bytes[counter] = byte;
        ++counter;
    }
    return bytes;
}

Sha1Hash hash::sha1(std::string const& strToHash) {
    boost::uuids::detail::sha1 sha;
    sha.process_bytes(strToHash.c_str(), strToHash.size());
    unsigned hash[5] = {0};
    sha.get_digest(hash);

    char buf[41] = {0};
    for (int i = 0; i < 5; i++)
    {
        std::sprintf(buf + (i << 3), "%08x", hash[i]);
    }

    auto hashBytes = stringHashToBytes(std::string(buf));
    return hashBytes;
}

std::string hash::urlEncodeSha1Hash(const hash::Sha1Hash &hash) {
    std::string output;
    for(auto el: hash) {
        int elNumber = static_cast<int>(el);
        if(elNumber >= 0x61 && elNumber <= 0x7a) {
            output.push_back('a' + elNumber - 0x61);
        }
        else if(elNumber >= 0x41 && elNumber <= 0x5a) {
            output.push_back('A' + elNumber - 0x41);
        }
        else if(elNumber >=0x30 && elNumber <= 0x39) {
            output.push_back('0' + elNumber - 0x30);
        }
        else if(elNumber == 0x20) {
            output.push_back('+');
        }
        else {
            std::stringstream ss;
            ss << std::hex << std::uppercase << elNumber;
            auto hexString = ss.str();
            output.push_back('%');
            if(hexString.size() == 1)
                output.push_back('0');
            output += hexString;
        }
    }
    return output;
}
