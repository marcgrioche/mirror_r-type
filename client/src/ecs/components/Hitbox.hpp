/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Hitbox Component
*/

#pragma once

struct Hitbox {
    float width = 1.0f;
    float height = 1.0f;
    float offset_x = 0.0f;
    float offset_y = 0.0f;
    // bool is_trigger = false;
    // uint32_t layer = 1u;      // bitmask layer
    // uint32_t mask = 0xFFFFFFFFu; // which layers this hitbox collides with
    // bool one_way = false;
};