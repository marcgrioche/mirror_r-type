/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** GravitySystem
*/

#include "RigidBodySystem.hpp"
#include "Config.hpp"
#include "components/Tags.hpp"
#include "components/componentutils/VectorUtils.hpp"
#include <algorithm>
#include <cmath>

void changeRigidBodyComponentProperties(
    RigidBody& t_rigidBody,
    Velocity& t_velocity,
    const float t_deltaTime,
    const bool t_isPlayer)
{
    if (!t_rigidBody.active) {
        return;
    }

    t_velocity.v += t_rigidBody.acceleration * t_deltaTime;

    if (t_rigidBody.isOnGround) {
        t_velocity.v.x *= (std::pow)(t_rigidBody.groundFriction, t_deltaTime);

        if (t_isPlayer) {
            if ((std::abs)(t_rigidBody.acceleration.x) < 1.0f) {
                t_velocity.v.x = 0.0f;
            }
        }
    } else {
        t_velocity.v.x *= (std::pow)(t_rigidBody.airFriction, t_deltaTime);
    }

    t_velocity.v.x = (std::clamp)(t_velocity.v.x, -t_rigidBody.maxSpeed, t_rigidBody.maxSpeed);

    // if (!t_rigidBody.isOnGround) {
    //     t_velocity.v.y += t_rigidBody.gravity;
    //     t_velocity.v.y = (std::min)(t_velocity.v.y, t_rigidBody.maxFallSpeed);
    // } else {
    //     t_velocity.v += t_rigidBody.groundSpeed;
    // }
    t_velocity.v.y += t_rigidBody.gravity;
    t_velocity.v.y = (std::min)(t_velocity.v.y, t_rigidBody.maxFallSpeed);
    if (t_rigidBody.isOnGround)
        t_velocity.v += t_rigidBody.groundSpeed;
}

void rigidBodySystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<RigidBody, Velocity>();

    for (auto it = view.begin(); it != view.end(); ++it) {
        auto [entity, rb, vel] = it.entity_and_components();
        bool isPlayer = false;
        if (registry.has<PlayerTag>(entity)) {
            isPlayer = true;
        }
        changeRigidBodyComponentProperties(rb, vel, deltaTime, isPlayer);
    }
}
