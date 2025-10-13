/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Lifetime Component - Time-to-live tracking for temporary entities
*/

#pragma once

/**
 * @brief Component representing remaining lifetime for temporary entities
 * @param value Remaining lifetime in seconds (entity is destroyed when <= 0)
 */
struct Lifetime {
    float value = 0.0f;
};
