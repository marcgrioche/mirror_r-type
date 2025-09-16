/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Components
*/

#pragma once
#include <cstddef>

using Entity = std::size_t;

struct Position {
    float x, y;
};
struct Velocity {
    float dx, dy;
};
struct Health {
    int hp;
};
