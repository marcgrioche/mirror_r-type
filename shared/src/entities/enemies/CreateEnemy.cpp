/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** enemy
*/
#include "CreateEnemy.hpp"
#include "../weapon/CreateWeapon.hpp"
#include "components/Position.hpp"
#include "components/PreviousPosition.hpp"
#include "components/Velocity.hpp"
#include "components/Health.hpp"
#include "components/Hitbox.hpp"
#include "components/Dead.hpp"
#include "components/Tags.hpp"
#include "../components/Frequency.hpp"
#include "../../ecs/systems/FrequencyUtils.hpp"
#include <iostream>

namespace factories {
Entity createEnemy(Registry& registry)
{
    std::cout << "\ncreating enemy\n\n";
    Entity enemy = registry.create_entity();
    registry.emplace<Position>(enemy, 700.0f, 250.0f);
    registry.emplace<PreviousPosition>(enemy, 50.0f, 480.0f);
    registry.emplace<Velocity>(enemy, 0.0f, 0.0f);
    registry.emplace<Health>(enemy, 15);
    registry.emplace<Hitbox>(enemy, 32.0f, 32.0f, 0.0f, 0.0f);
    registry.emplace<Dead>(enemy);
    registry.emplace<EnemyTag>(enemy);
    // registry.emplace<Sprite>(enemy, 0, 50, 50);
    // Create and customize enemy weapon (fire every 1.5s for example)
    Entity weapon = createWeapon(registry, Parent { enemy });
    if (registry.has<Frequency>(weapon)) {
        auto &freq = registry.get<Frequency>(weapon);
        freq.frequency = 1.5;                // seconds between shots (lower = faster)
        freq.lastTime = std::chrono::high_resolution_clock::now(); // delay first shot by full period
    }
    return enemy;
}

Entity createEnemy(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox)
{
    Entity enemy = registry.create_entity();
    registry.add<Position>(enemy, position);
    registry.add<PreviousPosition>(enemy, PreviousPosition { position.x, position.y });
    registry.add<Velocity>(enemy, Velocity { 0.0f, 0.0f });
    registry.add<Health>(enemy, health);
    registry.add<Hitbox>(enemy, hitbox);
    registry.add<Dead>(enemy, Dead {});
    registry.add<EnemyTag>(enemy, EnemyTag {});
    // registry.emplace<Sprite>(enemy, 0, 50, 50);
    Entity weapon = createWeapon(registry, Parent { enemy });
    if (registry.has<Frequency>(weapon)) {
        auto &freq = registry.get<Frequency>(weapon);
        freq.frequency = 1.5; // adjust as needed
        freq.lastTime = std::chrono::high_resolution_clock::now();
    }
    return enemy;
}
}
