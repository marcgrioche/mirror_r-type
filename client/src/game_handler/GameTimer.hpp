/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GameTimer class header
*/

#pragma once

#include <chrono>

/**
 * @brief Class for managing game timing and delta time calculations
 */
class GameTimer {
public:
    /**
     * @brief Constructs a new GameTimer
     */
    GameTimer();

    /**
     * @brief Starts the timer
     */
    void start();

    /**
     * @brief Gets the delta time since the last call
     * @return The time elapsed in seconds since the last getDeltaTime call
     */
    float getDeltaTime();

    /**
     * @brief Resets the timer
     */
    void reset();

private:
    std::chrono::high_resolution_clock::time_point _lastTime;
    std::chrono::high_resolution_clock::time_point _startTime;
    bool _isStarted;
};
