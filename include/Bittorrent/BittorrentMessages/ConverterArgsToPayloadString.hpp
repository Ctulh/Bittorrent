//
// Created by ctuh on 8/12/23.
//

#pragma once

#include "BittorrentMessageType.hpp"

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <vector>

#ifdef __linux__
#include <netinet/in.h>
#elif _WIN32

#else

#endif

template<MessageType>
class ConverterArgsToPayloadString {
public:
    ConverterArgsToPayloadString() {
        throw std::runtime_error("Not specialized MessageType");
    }
};

template<>
class ConverterArgsToPayloadString<MessageType::KEEP_ALIVE> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::string getPayload() {return "";}
};

template<>
class ConverterArgsToPayloadString<MessageType::CHOKE> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::string getPayload() {return "";}
};

template<>
class ConverterArgsToPayloadString<MessageType::UNCHOKE> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::string getPayload() {return "";}
};

template<>
class ConverterArgsToPayloadString<MessageType::INTERESTED> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::string getPayload() {return "";}
};

template<>
class ConverterArgsToPayloadString<MessageType::NOT_INTERESTED> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::string getPayload() {return "";}
};

template<>
class ConverterArgsToPayloadString<MessageType::HAVE> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::string getPayload(int pieceLength) {
        std::stringstream messageStream;
        messageStream << std::setw(4) << std::setfill('0') << pieceLength;
        return messageStream.str();
    }
};

template<>
class ConverterArgsToPayloadString<MessageType::BITFIELD> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::string getPayload(std::vector<std::byte> const& bitfield) {
        std::stringstream messageStream;
        const std::size_t n = bitfield.size();
        for(std::size_t i = 0; i < n; ++i) {
            messageStream << static_cast<char>(bitfield[i]);
        }
        return messageStream.str();
    }
};

template<>
class ConverterArgsToPayloadString<MessageType::REQUEST> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::string getPayload(int index, int begin, int length) {
        std::stringstream messageStream;
        messageStream << std::setw(4) << std::setfill('0') << index;
        messageStream << std::setw(4) << std::setfill('0') << begin;
        messageStream << std::setw(4) << std::setfill('0') << length;
        return messageStream.str();
    }
};

template<>
class ConverterArgsToPayloadString<MessageType::PIECE> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::string getPayload(int index, int begin, std::vector<char> const& block) {
        std::stringstream messageStream;
        messageStream << std::setw(4) << std::setfill('0') << index;
        messageStream << std::setw(4) << std::setfill('0') << begin;
        const std::size_t n = block.size();
        for(std::size_t i = 0; i < n; ++i) {
            messageStream << block[i];
        }
        return messageStream.str();
    }
};

template<>
class ConverterArgsToPayloadString<MessageType::CANCEL> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::string getPayload(int index, int begin, int length) {
        std::stringstream messageStream;
        messageStream << std::setw(4) << std::setfill('0') << index;
        messageStream << std::setw(4) << std::setfill('0') << begin;
        messageStream << std::setw(4) << std::setfill('0') << length;
        return messageStream.str();
    }
};

template<>
class ConverterArgsToPayloadString<MessageType::PORT> {
public:
    ConverterArgsToPayloadString () = delete;
public:
    static std::string getPayload(unsigned short port) {
        std::stringstream messageStream;
        port = htons(port);
        auto ptr = reinterpret_cast<std::byte*>(&port);
        messageStream << static_cast<char>(*ptr);
        ++ptr;
        messageStream << static_cast<char>(*ptr);
        return messageStream.str();
    }
};