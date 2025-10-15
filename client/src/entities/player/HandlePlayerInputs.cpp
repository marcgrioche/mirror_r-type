/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** HandlePlayerInputs
*/

#include "HandlePlayerInputs.hpp"
#include "components/Velocity.hpp"
#include "components/Tags.hpp"
#include "Config.hpp"
#include "components/RigidBody.hpp"
#include <iostream>

void handlePlayerInputs(InputManager& _inputs, Registry& registry)
{
    auto view = registry.view<PlayerTag, Velocity, RigidBody>();

    for (auto [tag, velocity, rigidBody] : view) {

        const float speed = 250.0f;

        velocity.v.x = 0.0f;

        if (_inputs.isUp()) {
            if (rigidBody.isOnGround) {
                velocity.v.y = -V0;
            }
        }
        if (_inputs.isLeft()) {
            velocity.v.x = -speed;
        }
        if (_inputs.isRight()) {
            velocity.v.x = speed;
        }

        if (_inputs.isDown() && !rigidBody.isOnGround && velocity.v.y > 0) {
            velocity.v.y += 300.0f;
        }
    }
}
