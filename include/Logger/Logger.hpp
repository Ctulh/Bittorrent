//
// Created by ctuh on 7/25/23.
//

#pragma once

#include "ILogger.hpp"

#include <unordered_map>
#include <memory>

class Logger {
private:
    Logger() = default;
public:
    static void addLogger(std::string const& loggerName, LoggerPtr logger);
    static void removeLogger(std::string loggerName);

    static void logInfo(std::string const& message);
    static void logWarning(std::string const& message);
    static void logError(std::string const& message);
    static void logCritical(std::string const& message);

private:
    static std::unordered_map<std::string, LoggerPtr> m_loggers;
};
