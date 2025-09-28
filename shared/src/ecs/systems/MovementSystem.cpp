/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** MovementSystem implementation
*/

#include "MovementSystem.hpp"
#include "components/AllComponents.hpp"
#include <random>


void movementPlatform(Registry& registry, float deltaTime)
{
    constexpr float ScreenH = 600.0f;
    constexpr float PlatformH = 20.0f;

    auto view = registry.view<Position, Velocity, PlatformTag>();
    for (auto&& [pos, vel, tag] : view) {
        pos.x += vel.dx * deltaTime;
        pos.y += vel.dy * deltaTime;

        if (pos.x < -120) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> distY(0.0f, ScreenH - PlatformH);

            float y = distY(gen);
            pos.x = 800;
            pos.y = y;
        }
    }
}

void movementSystem(Registry& registry, float deltaTime)
{
    auto prevPosView = registry.view<Position, PreviousPosition>();
    for (auto&& [pos, prevPos] : prevPosView) {
        prevPos.x = pos.x;
        prevPos.y = pos.y;
    }

    auto view = registry.view<Position, Velocity, PlayerTag>();
    for (auto&& [pos, vel, tag] : view) {
        pos.x += vel.dx * deltaTime;
        pos.y += vel.dy * deltaTime;

        if (pos.x < 0) {
            pos.x = 0;
            vel.dx = 0;
        }
        if (pos.x > 800 - 32) {
            pos.x = 800 - 32;
            vel.dx = 0;
        }
    }
    movementPlatform(registry, deltaTime);
}
