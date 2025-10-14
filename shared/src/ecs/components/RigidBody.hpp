/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** RigidBody Component - Physics properties for entities affected by gravity
*/

#pragma once
#include "Vector2.hpp"

/**
 * @brief Component representing physics properties for entities affected by gravity
 * @param weight Mass/weight of the entity affecting gravity response
 * @param isOnPlatform True if the entity is currently standing on a platform
 */

struct RigidBody {
    bool active = true; //can be disabled for special behaviors
    float weight = 1.f;
    float groundAccel = 10000.f;
    float airAccel = 2000.f;
    float groundFriction = 0.001f;
    float airFriction = 0.3f;
    float maxSpeed = 300.f; //mainly to limit input based speed ?
    float maxFallSpeed = 1500.f;
    float gravity = 100.f;
    bool isOnPlatform = false;
    Vector2 groundSpeed;
    Vector2 acceleration;
};
