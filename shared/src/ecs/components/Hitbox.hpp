/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Hitbox Component - Collision detection boundaries for entities
*/

#pragma once

/**
 * @brief Component defining collision boundaries for entities
 * @param width Width of the hitbox in pixels
 * @param height Height of the hitbox in pixels
 * @param offset_x X offset from entity position to hitbox center
 * @param offset_y Y offset from entity position to hitbox center
 */
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
