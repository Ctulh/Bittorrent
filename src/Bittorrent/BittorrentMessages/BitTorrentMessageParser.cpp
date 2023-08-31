#include "Bittorrent/BittorrentMessages/BittorrentMessageParser.hpp"

std::vector<BittorrentMessage> BittorrentMessageParser::getMessages(const std::vector<std::byte> &message) {
    std::vector<BittorrentMessage> output;
    std::size_t index = 0;
    if(hasBittorrentHandshakePart(message)) {
        index = 68; // because BitTorrentHandshake has length - 68 bytes
    }
    const std::size_t n = message.size();
    while(index < n) {
        std::vector<std::byte> lengthBytes;
        for (std::size_t i = index; i < n && i < index + 4; ++i) {
            lengthBytes.push_back((std::byte) (message[i]));
        }
        index += 4;
        const std::size_t length = ByteMethods::convertFourBytesToNumber(lengthBytes);

        BittorrentMessage bittorrentMessage;

        if(length == 0) {
            bittorrentMessage.setMessageType(MessageType::KEEP_ALIVE);
            output.push_back(bittorrentMessage);
            continue;
        }

        bittorrentMessage.setMessageType(static_cast<MessageType>(message[index]));
        ++index;

        if(index + length - 1 <= n) {
            bittorrentMessage.setPayload(std::vector<std::byte>(message.begin() + index, message.begin() + index + length - 1));
        }

        index += length-1;
        output.push_back(bittorrentMessage);
    }
    return output;
}

bool BittorrentMessageParser::hasBittorrentHandshakePart(const std::vector<std::byte> &msg) {
    const std::vector<std::byte> bittorrentInBytes {std::byte(0x42), std::byte(0x69), std::byte(0x74), std::byte(0x54), std::byte(0x6f), std::byte(0x72), std::byte(0x72), std::byte(0x65), std::byte(0x6e), std::byte(0x74)};
    std::size_t indexBittorrentWord = 0;
    std::size_t indexMsg = 0;
    const std::size_t n = msg.size();
    const std::size_t m = bittorrentInBytes.size();

    while(indexMsg < n && indexBittorrentWord < m) {
        if(bittorrentInBytes[indexBittorrentWord] == msg[indexMsg]) {
            ++indexBittorrentWord;
        } else {
            indexBittorrentWord = 0;
        }
        ++indexMsg;
    }
    return indexBittorrentWord == m;
}