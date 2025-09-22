/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GameTimer class header
*/

#pragma once

#include <chrono>

class GameTimer {
public:
    GameTimer();

    void start();
    float getDeltaTime();
    void reset();

private:
    std::chrono::high_resolution_clock::time_point _lastTime;
    std::chrono::high_resolution_clock::time_point _startTime;
    bool _isStarted;
};
