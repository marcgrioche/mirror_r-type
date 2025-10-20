/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** MovementSystem implementation
*/

#include "MovementSystem.hpp"
#include "../../include/Config.hpp"
#include "components/Dead.hpp"
#include "components/Health.hpp"
#include "components/Hitbox.hpp"
#include "components/Tags.hpp"
#include "components/Velocity.hpp"
#include "systems/FunctionMotionSystem.hpp"
#include <array>
#include <random>

int movementPlatform(Registry& registry, float /*deltaTime*/)
{
    int outPlatformCounter = 0;

    auto view = registry.view<Position, PlatformTag, Dead, Hitbox>();
    for (auto&& [pos, tag, dead, hitbox] : view) {
        if (!dead.dead && (pos.v.x + hitbox.width) < 0.0f) {
            dead.dead = true;
            ++outPlatformCounter;
        }
    }
    return outPlatformCounter;
}

void resetEntityPosition(const Position& t_position, PreviousPosition& t_previousPosition)
{
    t_previousPosition.v.x = t_position.v.x;
    t_previousPosition.v.y = t_position.v.y;
}

void changeMovementComponentProperties(Registry& t_registry, const Entity& t_entity, const float t_deltaTime)
{
    auto& position = t_registry.get<Position>(t_entity);
    auto& velocity = t_registry.get<Velocity>(t_entity);

    if (t_registry.has<PlayerTag>(t_entity) && t_registry.has<Health>(t_entity)) {
        const auto& health = t_registry.get<Health>(t_entity);
        if (health.hp <= 0) {
            return;
        }
    }

    position.v.x += velocity.v.x * t_deltaTime;
    position.v.y += velocity.v.y * t_deltaTime;
}

int movementSystem(Registry& registry, float deltaTime)
{
    // store previous positions for entities that need it
    auto prevPosView = registry.view<Position, PreviousPosition>();
    for (auto&& [pos, prevPos] : prevPosView) {
        resetEntityPosition(pos, prevPos);
    }

    functionMotionSystem(registry, deltaTime);

    auto view = registry.view<Position, Velocity>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        auto [entity, pos, vel] = it.entity_and_components();

        changeMovementComponentProperties(registry, entity, deltaTime);
    }

    int outPlatforms = movementPlatform(registry, deltaTime);
    return outPlatforms;
}
