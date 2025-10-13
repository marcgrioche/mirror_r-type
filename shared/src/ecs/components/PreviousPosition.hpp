/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** PreviousPosition - Store the previous frame position for collision detection
*/

#pragma once

/**
 * @brief Component storing the position from the previous frame for collision detection
 * @param x Previous X-coordinate position
 * @param y Previous Y-coordinate position
 */
struct PreviousPosition {
    float x = 0.0f;
    float y = 0.0f;
};
