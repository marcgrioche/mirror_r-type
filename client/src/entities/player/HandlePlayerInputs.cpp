/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** HandlePlayerInputs
*/

#include "HandlePlayerInputs.hpp"

void handlePlayerInputs(InputManager& _inputs, Registry& registry)
{
    auto view = registry.view<PlayerTag, Velocity>();

    for (auto [tag, velocity] : view) {
        velocity.dx = 0.0f;
        velocity.dy = 0.0f;
        const float speed = 200.0f; // pixels per second

        if (_inputs.isUp()) {
            velocity.dy = -speed;
            std::cout << "up\n";
        }
        if (_inputs.isDown()) {
            velocity.dy = speed;
            std::cout << "down\n";
        }
        if (_inputs.isLeft()) {
            velocity.dx = -speed;
            std::cout << "left\n";
        }
        if (_inputs.isRight()) {
            velocity.dx = speed;
            std::cout << "right\n";
        }
    }
}
