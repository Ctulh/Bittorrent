//
// Created by ctuh on 7/25/23.
//

#pragma once

#include "ILogger.hpp"

#include <spdlog/spdlog.h>
#include <memory>

class LoggerSpdlog: public ILogger {
public:
    explicit LoggerSpdlog(std::string const& name, std::string const& path);
public:
    void logInfo(const std::string &message) override;
    void logWarning(const std::string &message) override;
    void logError(const std::string &message) override;
    void logCritical(const std::string &message) override;

private:
    std::unique_ptr<spdlog::logger> m_logger;
};
