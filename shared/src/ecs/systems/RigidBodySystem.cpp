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

        vel = rb.acceleration * deltaTime;
        vel *= friction * deltaTime;

        vel.x = std::clamp(rb.velocity.x, -rb.maxSpeed, rb.maxSpeed);

        if (!rb.isOnPlatform) {
            rb.velocity.y *= rb.gravity * deltaTime;
            rb.velocity.y = std::min(rb.velocity.y, rb.maxFallSpeed);
        }
        // if (vel.dy < 0.f) {
        //     vel.dy += GRAVITY_UP * deltaTime;
        // } else {
        //     vel.dy += GRAVITY_DOWN * deltaTime;
        // }

        // if (vel.dy > MAX_FALL) {
        //     vel.dy = MAX_FALL;
        // }
    }
}
