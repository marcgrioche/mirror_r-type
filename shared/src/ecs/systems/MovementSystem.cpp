/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** MovementSystem implementation
*/

#include "MovementSystem.hpp"
#include "../../include/Config.hpp"
#include "components/AllComponents.hpp"
#include <array>
#include <random>

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
    for (auto it = view.begin(); it != view.end(); ++it) {
        auto [entity, pos, vel] = it.entity_and_components();

        if (registry.has<PlayerTag>(entity) && registry.has<Health>(entity)) {
            const auto& health = registry.get<Health>(entity);
            if (health.hp <= 0) {
                continue;
            }
        }

        pos.x += vel.dx * deltaTime;
        pos.y += vel.dy * deltaTime;
    }
    int outPlatforms = movementPlatform(registry, deltaTime);
    return outPlatforms;
}
