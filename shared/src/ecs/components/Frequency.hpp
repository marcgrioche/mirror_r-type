/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Frequency - Component for managing timed events and cooldowns
*/

#pragma once
#include <chrono>

/**
 * @brief Component that manages frequency-based timing for events and cooldowns
 * Tracks when events should occur based on time intervals and maintains last execution time
 */
struct Frequency {
    double frequency = 1.0; /**< Time interval in seconds between event occurrences */
    std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::time_point::min(); /**< Timestamp of last event execution */

    Frequency() = default;
    /**
     * @brief Constructor with frequency parameter
     * @param freq Time interval in seconds between events
     */
    Frequency(double freq)
        : frequency(freq)
    {
    }
};
