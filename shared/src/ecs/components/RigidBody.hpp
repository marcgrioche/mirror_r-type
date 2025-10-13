/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** RigidBody Component - Physics properties for entities affected by gravity
*/

#pragma once

/**
 * @brief Component representing physics properties for entities affected by gravity
 * @param weight Mass/weight of the entity affecting gravity response
 * @param IsOnPlatform True if the entity is currently standing on a platform
 */
struct RigidBody {
    float weight = 1;
    bool IsOnPlatform = false;
};
