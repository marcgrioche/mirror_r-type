/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** PreviousPosition - Store the previous frame position for collision detection
*/

#pragma once

#include "Vector2.hpp"

/**
 * @brief Component storing the position from the previous frame for collision detection
 * @param v Vector2 containing previous x and y coordinates
 */
struct PreviousPosition {
    Vector2 v;
};
