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
#include <cmath>
#include "components/Tags.hpp"
#include <cmath>

void rigidBodySystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<RigidBody, Velocity>();

    for (auto it = view.begin(); it != view.end(); ++it) {
        auto [entity, rb, vel] = it.entity_and_components();
        if (!rb.active) {
            continue;
        }
        
        vel.v += rb.acceleration * deltaTime;

        if (rb.isOnGround) {
            vel.v.x *= pow(rb.groundFriction, deltaTime);
            
            if (registry.has<PlayerTag>(entity)) {
                if (std::abs(rb.acceleration.x) < 1.0f) {
                    vel.v.x = 0.0f;
                }
            }
        } else {
            vel.v.x *= pow(rb.airFriction, deltaTime);
        }

        vel.v.x = std::clamp(vel.v.x, -rb.maxSpeed, rb.maxSpeed);

        if (!rb.isOnGround) {
            vel.v.y += rb.gravity;
            vel.v.y = std::min(vel.v.y, rb.maxFallSpeed);
        } else {
            vel.v += rb.groundSpeed;
        }
    }
}
