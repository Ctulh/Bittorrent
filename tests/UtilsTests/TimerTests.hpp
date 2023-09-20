//
// Created by ctuh on 9/20/23.
//

#pragma once

#include <gtest/gtest.h>
#include <thread>

#include "Utils/Timer.hpp"

TEST(TimerTest, TestTimerConstructFromMilliseconds) {
    EXPECT_NO_THROW(Timer timer(std::chrono::milliseconds(1000)));
}

TEST(TimerTest, TestTimerConstructFromSeconds) {
    EXPECT_NO_THROW(Timer timer(std::chrono::seconds(1)));
}

TEST(TimerTest, TestTimerExpireBeforeTimeout) {
    Timer timer(std::chrono::milliseconds(1000));

    EXPECT_FALSE(timer.isExpired());
}

TEST(TimerTest, TestTimerExpireAfterTimeout) {
    Timer timer(std::chrono::milliseconds(1000));

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    EXPECT_TRUE(timer.isExpired());
}

TEST(TimerTest, TestTimerResetBeforeTimeout) {
    Timer timer(std::chrono::milliseconds(1000));

    EXPECT_FALSE(timer.isExpired());
    timer.reset();
    EXPECT_FALSE(timer.isExpired());
}

TEST(TimerTest, TestTimerResetAfterTimeout) {
    Timer timer(std::chrono::milliseconds(1000));

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    EXPECT_TRUE(timer.isExpired());
    timer.reset();

    EXPECT_FALSE(timer.isExpired());
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    EXPECT_TRUE(timer.isExpired());
}



