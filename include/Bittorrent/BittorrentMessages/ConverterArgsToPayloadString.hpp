//
// Created by ctuh on 8/12/23.
//

#pragma once

#include "BittorrentMessageType.hpp"
#include "Utils/ByteMethods.hpp"

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

#ifdef __linux__
#include <netinet/in.h>
#elif _WIN32

#else

#endif

/*!
 * @brief This class shouldn't be instantiated, but if this happened it will throw std::runtime_error.
 *        In the good way full specialization should be called. See tests for clarification.
 *
 *        I think this class will allow you to not have to remember every payload for every message type.
 *        Just write your message type in the template and the IDE will show you what parameters you need to build
 *        the payload for that message type.
 */
template<MessageType>
class ConverterArgsToPayloadString {
public:
    ConverterArgsToPayloadString() {
        throw std::runtime_error("Not specialized MessageType");
    }
};

/*!
 * @brief Full specialization for 'KEEP_ALIVE' MessageType
*/
template<>
class ConverterArgsToPayloadString<MessageType::KEEP_ALIVE> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::vector<std::byte> getPayload() {return {};}
};

/*!
 * @brief Full specialization for 'CHOKE' MessageType
*/
template<>
class ConverterArgsToPayloadString<MessageType::CHOKE> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::vector<std::byte> getPayload() {return {};}
};

/*!
 * @brief Full specialization for 'UNCHOKE' MessageType
*/
template<>
class ConverterArgsToPayloadString<MessageType::UNCHOKE> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::vector<std::byte> getPayload() {return {};}
};

/*!
 * @brief Full specialization for 'INTERESTED' MessageType
*/
template<>
class ConverterArgsToPayloadString<MessageType::INTERESTED> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::vector<std::byte> getPayload() {return {};}
};

/*!
 * @brief Full specialization for 'NOT_INTERESTED' MessageType
*/
template<>
class ConverterArgsToPayloadString<MessageType::NOT_INTERESTED> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::vector<std::byte> getPayload() {return {};}
};

/*!
 * @brief Full specialization for 'HAVE' MessageType
*/
template<>
class ConverterArgsToPayloadString<MessageType::HAVE> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::vector<std::byte> getPayload(int pieceLength) {
        return ByteMethods::convertNumberToFourBytes(pieceLength);
    }
};

/*!
 * @brief Full specialization for 'BITFIELD' MessageType
*/
template<>
class ConverterArgsToPayloadString<MessageType::BITFIELD> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::vector<std::byte> getPayload(std::vector<std::byte> const& bitfield) {
        return bitfield;
    }
};

/*!
 * @brief Full specialization for 'REQUEST' MessageType
*/
template<>
class ConverterArgsToPayloadString<MessageType::REQUEST> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::vector<std::byte> getPayload(int index, int begin, int length) {
        auto indexBytes = ByteMethods::convertNumberToFourBytes(index);
        auto beginBytes = ByteMethods::convertNumberToFourBytes(begin);
        auto lengthBytes = ByteMethods::convertNumberToFourBytes(length);

        std::vector<std::byte> message;

        message.insert(message.end(), indexBytes.begin(), indexBytes.end());
        message.insert(message.end(), beginBytes.begin(), beginBytes.end());
        message.insert(message.end(), lengthBytes.begin(), lengthBytes.end());

        return message;
    }
};

/*!
 * @brief Full specialization for 'PIECE' MessageType
*/
template<>
class ConverterArgsToPayloadString<MessageType::PIECE> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::vector<std::byte> getPayload(int index, int begin, std::vector<std::byte> const& block) {
        auto indexBytes = ByteMethods::convertNumberToFourBytes(index);
        auto beginBytes = ByteMethods::convertNumberToFourBytes(begin);

        std::vector<std::byte> message;
        message.insert(message.end(), indexBytes.begin(), indexBytes.end());
        message.insert(message.end(), beginBytes.begin(), beginBytes.end());
        message.insert(message.end(), block.begin(), block.end());
        return message;
    }
};

/*!
 * @brief Full specialization for 'CANCEL' MessageType
*/
template<>
class ConverterArgsToPayloadString<MessageType::CANCEL> {
public:
    ConverterArgsToPayloadString() = delete;
public:
    static std::vector<std::byte> getPayload(int index, int begin, int length) {
        auto indexBytes = ByteMethods::convertNumberToFourBytes(index);
        auto beginBytes = ByteMethods::convertNumberToFourBytes(begin);
        auto LengthBytes = ByteMethods::convertNumberToFourBytes(length);

        std::vector<std::byte> message;

        message.insert(message.end(), indexBytes.begin(), indexBytes.end());
        message.insert(message.end(), beginBytes.begin(), beginBytes.end());
        message.insert(message.end(), LengthBytes.begin(), LengthBytes.end());

        return message;
    }
};

/*!
 * @brief Full specialization for 'PORT' MessageType
*/
template<>
class ConverterArgsToPayloadString<MessageType::PORT> {
public:
    ConverterArgsToPayloadString () = delete;
public:
    static std::vector<std::byte> getPayload(unsigned short port) {
        std::vector<std::byte> message;
        port = htons(port);
        auto ptr = reinterpret_cast<std::byte*>(&port);
        message.push_back(*ptr);
        ++ptr;
        message.push_back(*ptr);
        return message;
    }
};
