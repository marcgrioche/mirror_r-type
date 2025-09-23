/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** HandlePlayerInputs
*/

#include "HandlePlayerInputs.hpp"

void handlePlayerInputs(InputManager& _inputs, Registry& registry)
{
    auto view = registry.view<InputControlled, Velocity>();

    for (auto [inputCtrl, velocity] : view) {
        velocity.dx = 0.0f;
        velocity.dy = 0.0f;
        const float speed = 200.0f; // pixels per second

        if (_inputs.isUp()) {
            velocity.dy = -speed;
        }
        if (_inputs.isDown()) {
            velocity.dy = speed;
        }
        if (_inputs.isLeft()) {
            velocity.dx = -speed;
        }
        if (_inputs.isRight()) {
            velocity.dx = speed;
        }
    }
}
