//
// Created by ctuh on 9/25/23.
//

#include "Bittorrent/MessageHandler.hpp"
#include "Logger/Logger.hpp"
#include "Bittorrent/BittorrentMessages/BitTorrentMessageTypeConverter.hpp"

#include <format>

void MessageHandler::setHandler(MessageType messageType, BitTorrentMessageHandlerType handler) {
    if(m_typeToHandler.contains(messageType)) {
        Logger::logWarning(std::format("Redefining handler for type: {}", BitTorrentMessageTypeConverter::messageTypeToString(messageType)));
    }
    m_typeToHandler[messageType] = handler;
}

void MessageHandler::handle(const std::string &host, const BittorrentMessage &message) {
    if(m_typeToHandler.contains(message.getMessageType()) == 0) {
        Logger::logWarning(std::format("No handler for type: {}", BitTorrentMessageTypeConverter::messageTypeToString(message.getMessageType())));
        return;
    }

    m_typeToHandler[message.getMessageType()](host, message.getPayload());
}
