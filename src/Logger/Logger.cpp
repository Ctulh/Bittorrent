#include "Logger.hpp"


void Logger::addLogger(std::string const& loggerName, LoggerPtr logger) {
    m_loggers[loggerName] = logger;
}

void Logger::removeLogger(std::string loggerName) {
    if(m_loggers.count(loggerName))
        m_loggers.erase(loggerName);
}


void Logger::logInfo(std::string const& message) {
    for(auto&& [loggerName, logger]: m_loggers) {
        logger->logInfo(message);
    }
}

void Logger::logWarning(std::string const& message) {
    for(auto&& [loggerName, logger]: m_loggers) {
        logger->logWarning(message);
    }
}

void Logger::logError(std::string const& message) {
    for(auto&& [loggerName, logger]: m_loggers) {
        logger->logError(message);
    }
}

void Logger::logCritical(std::string const& message) {
    for(auto&& [loggerName, logger]: m_loggers) {
        logger->logCritical(message);
    }
}

std::unordered_map<std::string, LoggerPtr> Logger::m_loggers;
