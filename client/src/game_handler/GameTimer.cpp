/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GameTimer implementation
*/

#include "GameTimer.hpp"

GameTimer::GameTimer()
    : _isStarted(false)
{
}

void GameTimer::start()
{
    _startTime = std::chrono::high_resolution_clock::now();
    _lastTime = _startTime;
    _isStarted = true;
}

float GameTimer::getDeltaTime()
{
    if (!_isStarted) {
        start();
        return 0.0f;
    }

    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - _lastTime).count();
    _lastTime = currentTime;

    return deltaTime;
}

void GameTimer::reset()
{
    _isStarted = false;
}
