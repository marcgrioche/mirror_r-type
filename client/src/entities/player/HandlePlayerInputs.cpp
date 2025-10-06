/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** HandlePlayerInputs
*/

#include "HandlePlayerInputs.hpp"
#include "Jump.hpp"
#include <iostream>

void handlePlayerInputs(InputManager& _inputs, Registry& registry)
{
    auto view = registry.view<PlayerTag, Velocity, Jump>();

    for (auto [tag, velocity, jump] : view) {

        const float speed = 250.0f;

        velocity.dx = 0.0f;

        if (_inputs.isUp()) {
            if (!jump.isJumping && jump.canJump) {
                velocity.dy = -V0;
                jump.isJumping = true;
                jump.canJump = false;
            }
        }
        if (_inputs.isLeft()) {
            velocity.dx = -speed;
        }
        if (_inputs.isRight()) {
            velocity.dx = speed;
        }

        if (_inputs.isDown() && jump.isJumping && velocity.dy > 0) {
            velocity.dy += 300.0f;
        }
    }
}
