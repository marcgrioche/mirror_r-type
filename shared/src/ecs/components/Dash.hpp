/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Dash Component - Handles player dash ability
*/

#pragma once
#include "Frequency.hpp"
#include "Vector2.hpp"

/**
 * @brief Component that manages player dash mechanics and state
 * Tracks dash duration, cooldown, speed, and directional movement during dash ability
 */
struct Dash {
    float duration = 0.08f; /**< Total duration of the dash in seconds */
    float remaining = 0.0f; /**< Remaining time left in current dash */
    bool isDashing = false; /**< Whether the entity is currently dashing */
    float dashSpeed = 1000.0f; /**< Speed multiplier during dash (pixels per second) */
    Frequency cooldown = Frequency { 0.5 }; /**< Cooldown between dashes (0.5 seconds) */
    Vector2 direction = { 0, 0 }; /**< Normalized direction vector of the dash */
};
