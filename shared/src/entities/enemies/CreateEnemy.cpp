/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** enemy
*/
#include "CreateEnemy.hpp"
#include "../weapon/CreateWeapon.hpp"

namespace factories {
Entity createEnemy(std::shared_ptr<Registry> registry)
{
    std::cout << "\ncreating enemy\n\n";
    Entity enemy = registry->create_entity();
    registry->emplace<Position>(enemy, 700.0f, 250.0f);
    registry->emplace<PreviousPosition>(enemy, 50.0f, 480.0f);
    registry->emplace<Velocity>(enemy, 0.0f, 0.0f);
    registry->emplace<Health>(enemy, 100);
    registry->emplace<Hitbox>(enemy, 32.0f, 32.0f, 0.0f, 0.0f);
    registry->emplace<EnemyTag>(enemy);
    // registry.emplace<Sprite>(enemy, 0, 50, 50);
    createWeapon(registry, Parent { enemy });
    return enemy;
}

Entity createEnemy(std::shared_ptr<Registry> registry, const Position& position, const Health& health, const Hitbox& hitbox)
{
    Entity enemy = registry->create_entity();
    registry->add<Position>(enemy, position);
    registry->add<PreviousPosition>(enemy, PreviousPosition { position.x, position.y });
    registry->add<Velocity>(enemy, Velocity { 0.0f, 0.0f });
    registry->add<Health>(enemy, health);
    registry->add<Hitbox>(enemy, hitbox);
    registry->add<EnemyTag>(enemy, EnemyTag {});
    // registry.emplace<Sprite>(enemy, 0, 50, 50);
    createWeapon(registry, Parent { enemy });
    return enemy;
}
}
