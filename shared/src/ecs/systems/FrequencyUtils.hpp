/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** FrequencyUtils
*/

#pragma once
#include "Frequency.hpp"
#include <chrono>
#include <iostream>

namespace FrequencyUtils {

    // Check if enough time has passed for the frequency to trigger
    inline bool shouldTrigger(const Frequency& freq) {
        auto now = std::chrono::high_resolution_clock::now();
        
        // If lastTime is min, it means we want immediate trigger
        if (freq.lastTime == std::chrono::high_resolution_clock::time_point::min()) {
            return true;
        }
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - freq.lastTime);
        std::cout << elapsed.count() << std::endl;
        return elapsed.count() >= freq.frequency;
    }

    // Reset the frequency timer
    inline void reset(Frequency& freq) {
        freq.lastTime = std::chrono::high_resolution_clock::now();
    }

    // Check and reset if triggered
    inline bool triggerAndReset(Frequency& freq) {
        if (shouldTrigger(freq)) {
            reset(freq);
            return true;
        }
        return false;
    }

    // Force the frequency to be ready for immediate trigger
    inline void makeReady(Frequency& freq) {
        freq.lastTime = std::chrono::high_resolution_clock::time_point::min();
    }
}
