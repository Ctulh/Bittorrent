//
// Created by ctuh on 9/20/23.
//

#include "Utils/Timer.hpp"

Timer::Timer(std::chrono::milliseconds timeout): m_timeout(timeout) {
    m_expirationTime = std::chrono::system_clock::now() + m_timeout;
}

bool Timer::isExpired() const {
    return std::chrono::system_clock::now() >= m_expirationTime;
}

void Timer::reset() {
    m_expirationTime = std::chrono::system_clock::now() + m_timeout;
}

void Timer::changeTimeout(std::chrono::milliseconds timeout) {
    m_timeout = timeout;
    m_expirationTime = std::chrono::system_clock::now() + m_timeout;
}
