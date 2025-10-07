/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Dash Component - Handles player dash ability
*/

#pragma once
#include "Frequency.hpp"

struct Dash {
    float duration;        // Total duration of the dash (0.2 seconds)
    float remaining;       // Remaining time of current dash
    bool isDashing;        // Whether currently dashing
    float dashSpeed;       // Speed during dash
    Frequency cooldown;    // Cooldown between dashes (uses Frequency)
    int direction;         // Direction of dash: -1 (left), 1 (right)
};
