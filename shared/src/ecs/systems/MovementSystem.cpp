/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** MovementSystem implementation
*/

#include "MovementSystem.hpp"
#include "components/AllComponents.hpp"
#include <random>
#include <array>

int movementPlatform(Registry& registry, float /*deltaTime*/)
{
    int outPlatformCounter = 0;

    auto view = registry.view<Position, PlatformTag, Dead, Hitbox>();
    for (auto&& [pos, tag, dead, hitbox] : view) {
        if (!dead.dead && (pos.x + hitbox.width) < 0.0f) {
            dead.dead = true;
            ++outPlatformCounter;
        }
    }
    return outPlatformCounter;
}


int movementSystem(Registry& registry, float deltaTime)
{
    // store previous positions for entities that need it
    auto prevPosView = registry.view<Position, PreviousPosition>();
    for (auto&& [pos, prevPos] : prevPosView) {
        prevPos.x = pos.x;
        prevPos.y = pos.y;
    }

    auto view = registry.view<Position, Velocity>();
    for (auto&& [pos, vel] : view) {
        pos.x += vel.dx * deltaTime;
        pos.y += vel.dy * deltaTime;

    }
    int outPlatforms = movementPlatform(registry, deltaTime);
    if (outPlatforms > 0) {
        std::cout << "Platforms left screen this tick: " << outPlatforms << std::endl;
    }
    return outPlatforms;
}
