/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GravitySystem
*/

#include "RigidBodySystem.hpp"
#include "components/RigidBody.hpp"
#include "components/Velocity.hpp"
#include "Config.hpp"
#include "components/componentutils/VectorUtils.hpp"
#include <algorithm>

void rigidBodySystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<RigidBody, Velocity>();

    for (auto&& [rb, vel] : view) {
        if (!rb.active) {
            continue;
        }

        float friction = rb.isOnPlatform ? rb.groundFriction : rb.airFriction;

        vel.v = rb.acceleration * deltaTime;
        vel.v = vel.v * friction * deltaTime;

        vel.v.x = std::clamp(vel.v.x, -rb.maxSpeed, rb.maxSpeed);

        if (!rb.isOnPlatform) {
            vel.v.y += rb.gravity * deltaTime;
            vel.v.y = std::min(vel.v.y, rb.maxFallSpeed);
        }
        // if (vel.v.y < 0.f) {
        //     vel.v.y += GRAVITY_UP * deltaTime;
        // } else {
        //     vel.v.y += GRAVITY_DOWN * deltaTime;
        // }

        // if (vel.v.y > MAX_FALL) {
        //     vel.v.y = MAX_FALL;
        // }
    }
}
