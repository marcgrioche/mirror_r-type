/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** enemy
*/
#include "CreateBoss.hpp"

namespace factories {
Entity createBoss(Registry& registry)
{
    Entity boss = createEnemy(registry);
    return boss;
}

Entity createBoss(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox, const Velocity& velocity)
{
    Entity boss = createEnemy(registry, position, health, hitbox, velocity);
    Entity enemy = factories::createEnemyTemplate(
        registry,
        Health {ENEMY_HEALTH},
        Hitbox {ENEMY_WIDTH, ENEMY_HEIGHT},
        Velocity {ENEMY_VELOCITY_X, ENEMY_VELOCITY_Y}
    );
    Entity weapon = createWeapon(registry, Parent {boss}, enemy);
    if (registry.has<Frequency>(weapon)) {
        auto &freq = registry.get<Frequency>(weapon);
        freq.frequency = 3;
        freq.lastTime = std::chrono::high_resolution_clock::now();
    }
    return boss;
}
}
