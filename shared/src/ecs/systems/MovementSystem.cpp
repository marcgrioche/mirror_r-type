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

// Detect platforms that left the screen (completely) on the left side
// and mark them as dead. Returns how many were marked this frame.
int movementPlatform(Registry& registry, float /*deltaTime*/)
{
    int outPlatformCounter = 0;

    // We also fetch the hitbox to know the width so we only flag once the platform is fully gone
    auto view = registry.view<Position, PlatformTag, Dead, Hitbox>();
    for (auto&& [pos, tag, dead, hitbox] : view) {
        // Consider the platform out only when its right edge is past x=0
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
    // Debug trace (can be silenced later)
    if (outPlatforms > 0) {
        std::cout << "Platforms left screen this tick: " << outPlatforms << std::endl;
    }
    return outPlatforms;
}
