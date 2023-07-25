//
// Created by ctuh on 7/25/23.
//

#pragma once

#include <string>
#include <memory>

class ILogger {
public:
    virtual ~ILogger() = default;

public:
    virtual void logInfo(std::string const& message) = 0;
    virtual void logWarning(std::string const& message) = 0;
    virtual void logError(std::string const& message) = 0;
    virtual void logCritical(std::string const& message) = 0;
};

using LoggerPtr = std::shared_ptr<ILogger>;
