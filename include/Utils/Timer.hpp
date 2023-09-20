//
// Created by ctuh on 9/20/23.
//

#pragma once

#include <chrono>

class Timer {
public:
    Timer(std::chrono::milliseconds timeout);

    template<typename T>
    Timer(std::chrono::duration<double, T> timeout): m_timeout(std::chrono::duration_cast<std::chrono::milliseconds>(timeout)) {
        m_expirationTime = std::chrono::system_clock::now() + m_timeout;
    }
public:

    template<typename T>
    void changeTimeout(std::chrono::duration<double, T> timeout) {
        m_timeout = std::chrono::duration_cast<std::chrono::milliseconds>(timeout);
    }

    bool isExpired() const;
    void reset();
    void changeTimeout(std::chrono::milliseconds timeout);

private:
    std::chrono::milliseconds m_timeout;
    std::chrono::time_point<std::chrono::system_clock> m_expirationTime;
};