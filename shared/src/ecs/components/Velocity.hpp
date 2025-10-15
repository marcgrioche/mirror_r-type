/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Velocity Component - Movement velocity for entities
*/

#pragma once

#include "Vector2.hpp"

/**
 * @brief Component representing movement velocity in 2D space
 * @param v Vector2 containing velocity in x and y directions (pixels per second)
 */
struct Velocity {
    Vector2 v;
};
