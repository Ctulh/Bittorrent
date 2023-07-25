//
// Created by ctuh on 7/25/23.
//

#pragma once

#include <gtest/gtest.h>
#include <mutex>

#include "ILogger.hpp"
#include "Logger.hpp"

class LoggerMock: public ILogger {
public:
    void logInfo(const std::string &message) override {
        m_lastMessage = message + "info";
    }
    void logWarning(const std::string &message) override {
        m_lastMessage = message + "warning";
    }
    void logError(const std::string &message) override {
        m_lastMessage = message + "error";
    }
    void logCritical(const std::string &message) override {
        m_lastMessage = message + "critical";
    }

    std::string getLastLoggedMessage() const {
        return m_lastMessage;
    }

private:
    std::string m_lastMessage;
};

class GlobalLoggerTest: public ::testing::Test {
    void SetUp() override {
        logger = std::make_shared<LoggerMock>();
        Logger::addLogger(loggerName, logger);
    }

    void TearDown() override {
        Logger::removeLogger(loggerName);
    }

protected:
    std::shared_ptr<LoggerMock> logger;
    const std::string loggerName = "loggerTest";
};

TEST(LoggerTests, testMockLoggerInfoMethod) {
    std::string const messageForLog = "someMessage";
    LoggerMock loggerMock;

    ASSERT_NO_THROW(loggerMock.logInfo(messageForLog));
    ASSERT_EQ(loggerMock.getLastLoggedMessage(), messageForLog + "info");
}

TEST(LoggerTests, testMockLoggerWarningMethod) {
    std::string const messageForLog = "someMessage";
    LoggerMock loggerMock;

    ASSERT_NO_THROW(loggerMock.logWarning(messageForLog));
    ASSERT_EQ(loggerMock.getLastLoggedMessage(), messageForLog + "warning");
}

TEST(LoggerTests, testMockLoggerErrorMethod) {
    std::string const messageForLog = "someMessage";
    LoggerMock loggerMock;

    ASSERT_NO_THROW(loggerMock.logError(messageForLog));
    ASSERT_EQ(loggerMock.getLastLoggedMessage(), messageForLog + "error");
}

TEST(LoggerTests, testMockLoggerCriticalMethod) {
    std::string const messageForLog = "someMessage";
    LoggerMock loggerMock;

    ASSERT_NO_THROW(loggerMock.logCritical(messageForLog));
    ASSERT_EQ(loggerMock.getLastLoggedMessage(), messageForLog + "critical");
}

TEST_F(GlobalLoggerTest, testMockLoggerInfoMethodFromGlobalLogger) {
    std::string const messageForLog = "someMessage";

    ASSERT_TRUE(logger->getLastLoggedMessage().empty());
    ASSERT_NO_THROW(Logger::logInfo(messageForLog));
    ASSERT_EQ(logger->getLastLoggedMessage(), messageForLog + "info");
}

TEST_F(GlobalLoggerTest, testMockLoggerWarningMethodFromGlobalLogger) {
    std::string const messageForLog = "someMessage";

    ASSERT_TRUE(logger->getLastLoggedMessage().empty());
    ASSERT_NO_THROW(Logger::logWarning(messageForLog));
    ASSERT_EQ(logger->getLastLoggedMessage(), messageForLog + "warning");
}

TEST_F(GlobalLoggerTest, testMockLoggerErrorMethodFromGlobalLogger) {
    std::string const messageForLog = "someMessage";

    ASSERT_TRUE(logger->getLastLoggedMessage().empty());
    ASSERT_NO_THROW(Logger::logError(messageForLog));
    ASSERT_EQ(logger->getLastLoggedMessage(), messageForLog + "error");
}

TEST_F(GlobalLoggerTest, testMockLoggerCriticalMethodFromGlobalLogger) {
    std::string const messageForLog = "someMessage";

    ASSERT_TRUE(logger->getLastLoggedMessage().empty());
    ASSERT_NO_THROW(Logger::logCritical(messageForLog));
    ASSERT_EQ(logger->getLastLoggedMessage(), messageForLog + "critical");
}

