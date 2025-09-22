/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Components
*/

#pragma once

struct Position {
    float x, y;
};

struct Velocity {
    float dx, dy;
};

struct Health {
    int hp;
};

// Tag components for future use
struct Player { };
struct Enemy { };
struct Projectile { };
