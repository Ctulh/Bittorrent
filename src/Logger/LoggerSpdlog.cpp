#include "LoggerSpdlog.hpp"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

LoggerSpdlog::LoggerSpdlog(const std::string &name, const std::string &path) {
    auto stdoutLogger = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    auto fileLogger = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(path, 1048576, 7);
    fileLogger->set_level(spdlog::level::debug);

    m_logger = std::make_unique<spdlog::logger>(name, std::initializer_list<spdlog::sink_ptr>{stdoutLogger, fileLogger});
}

void LoggerSpdlog::logInfo(std::string const& message) {
    m_logger->info(message);
    m_logger->flush();
}

void LoggerSpdlog::logError(std::string const& message) {
    m_logger->error(message);
    m_logger->flush();
}

void LoggerSpdlog::logWarning(std::string const& message) {
    m_logger->warn(message);
    m_logger->flush();
}

void LoggerSpdlog::logCritical(std::string const& message) {
    m_logger->critical(message);
    m_logger->flush();
}
