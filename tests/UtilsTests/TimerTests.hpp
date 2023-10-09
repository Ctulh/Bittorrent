//
// Created by ctuh on 9/20/23.
//

#pragma once

#include <gtest/gtest.h>
#include <thread>

#include "Utils/Timer.hpp"

TEST(TimerTest, TestTimerConstruct) {
    EXPECT_NO_THROW(Timer timer);
}

TEST(TimerTest, TestTimerExpireBeforeTimeout) {
    Timer timer;
    timer.setTimeout(std::chrono::milliseconds(1000));

    EXPECT_FALSE(timer.isExpired());
}

TEST(TimerTest, TestTimerExpireAfterTimeout) {
    Timer timer;
    timer.setTimeout(std::chrono::milliseconds(1000));

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    EXPECT_TRUE(timer.isExpired());
}

TEST(TimerTest, TestTimerChangeTimeoutBeforeTimeout) {
    Timer timer;
    timer.setTimeout(std::chrono::milliseconds(1000));

    EXPECT_FALSE(timer.isExpired());
    EXPECT_NO_THROW(timer.setTimeout(std::chrono::milliseconds(2000)));

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    EXPECT_FALSE(timer.isExpired());
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 + 100));
    EXPECT_TRUE(timer.isExpired());
}

TEST(TimerTest, TestTimerChangeTimeoutAfterTimeout) {
    Timer timer;
    timer.setTimeout(std::chrono::milliseconds(1000));

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 + 100));
    EXPECT_TRUE(timer.isExpired());
    EXPECT_NO_THROW(timer.setTimeout(std::chrono::milliseconds(2000)));
    EXPECT_FALSE(timer.isExpired());

    std::this_thread::sleep_for(std::chrono::milliseconds(2000 + 100));

    EXPECT_TRUE(timer.isExpired());
}
