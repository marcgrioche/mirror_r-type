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
#include "RigidBody.hpp"
#include <iostream>

void handlePlayerInputs(InputManager& _inputs, Registry& registry)
{
    auto view = registry.view<PlayerTag, Velocity, RigidBody>();

    for (auto [tag, velocity, rigidBody] : view) {

        const float speed = 250.0f;

        velocity.dx = 0.0f;

        if (_inputs.isUp()) {
            if (rigidBody.IsOnPlatform) {
                velocity.dy = -V0;
            }
        }
        if (_inputs.isLeft()) {
            velocity.dx = -speed;
        }
        if (_inputs.isRight()) {
            velocity.dx = speed;
        }

        if (_inputs.isDown() && !rigidBody.IsOnPlatform && velocity.dy > 0) {
            velocity.dy += 300.0f;
        }
    }
}
