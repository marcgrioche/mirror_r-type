/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** FrequencyUtils - Utility functions for managing frequency-based timing
*/

#pragma once
#include "Frequency.hpp"
#include <chrono>
#include <iostream>

namespace FrequencyUtils {

/**
 * @brief Check if enough time has passed for the frequency to trigger
 * @param freq The frequency component to check
 * @return true if the frequency interval has elapsed, false otherwise
 */
inline bool shouldTrigger(const Frequency& freq)
{
    auto now = std::chrono::high_resolution_clock::now();

    // If lastTime is min, it means we want immediate trigger
    if (freq.lastTime == (std::chrono::high_resolution_clock::time_point::min)()) {
        return true;
    }
    auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - freq.lastTime);
    return elapsed.count() >= freq.frequency;
}

/**
 * @brief Reset the frequency timer to the current time
 * @param freq The frequency component to reset
 */
inline void reset(Frequency& freq)
{
    freq.lastTime = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Check if frequency should trigger and reset timer if it does
 * @param freq The frequency component to check and potentially reset
 * @return true if triggered and reset, false otherwise
 */
inline bool triggerAndReset(Frequency& freq)
{
    if (shouldTrigger(freq)) {
        reset(freq);
        return true;
    }
    return false;
}

/**
 * @brief Force the frequency to be ready for immediate trigger
 * Sets lastTime to minimum value so next check will always return true
 * @param freq The frequency component to make ready
 */
inline void makeReady(Frequency& freq)
{
    freq.lastTime = (std::chrono::high_resolution_clock::time_point::min)();
}
}
