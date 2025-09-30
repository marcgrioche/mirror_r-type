/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Frequency
*/

#pragma once
#include <chrono>

struct Frequency {
    double frequency = 1.0; // happens every "frequency" seconds
    std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::time_point::min();
    
    Frequency() = default;
    Frequency(double freq) : frequency(freq) {}
};
