//
// Created by ctuh on 9/20/23.
//

#pragma once

#include <chrono>

class Timer {
public:
    Timer();

public:
    void setTimeout(std::chrono::milliseconds timeout);
    bool isExpired() const;

private:
    std::chrono::time_point<std::chrono::system_clock> m_expirationTime;
};
