//
// Created by ctuh on 9/20/23.
//

#include "Utils/Timer.hpp"

Timer::Timer(): m_expirationTime(std::chrono::system_clock::now()) {}

bool Timer::isExpired() const {
    return std::chrono::system_clock::now() >= m_expirationTime;
}

void Timer::setTimeout(std::chrono::milliseconds timeout) {
    m_expirationTime = std::chrono::system_clock::now() + timeout;
}
