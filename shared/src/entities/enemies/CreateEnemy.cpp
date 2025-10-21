/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** enemy
*/
#include "CreateEnemy.hpp"
#include "levels/Level.hpp"

namespace factories {
Entity createEnemy(Registry& registry, const Level* level)
{
    std::cout << "\ncreating enemy\n\n";
    Entity enemy = registry.create_entity();
    float velocityX = level ? level->getEnemyVelocityX() : ENEMY_VELOCITY_X;
    float velocityY = level ? level->getEnemyVelocityY() : ENEMY_VELOCITY_Y;
    int health = level ? level->getEnemyHealth() : ENEMY_HEALTH;
    float width = level ? level->getEnemyWidth() : ENEMY_WIDTH;
    float height = level ? level->getEnemyHeight() : ENEMY_HEIGHT;
    registry.emplace<Position>(enemy, 700.0f, 250.0f);
    registry.emplace<PreviousPosition>(enemy, 50.0f, 480.0f);
    registry.emplace<Velocity>(enemy, velocityX, velocityY);
    registry.emplace<Health>(enemy, health);
    registry.emplace<Hitbox>(enemy, width, height, 0.0f, 0.0f);
    registry.emplace<Dead>(enemy);
    registry.emplace<EnemyTag>(enemy);
    // registry.emplace<Sprite>(enemy, 0, 50, 50);

    Entity projectile = factories::createProjectileTemplate(
        registry,
        Velocity { PROJECTILE_VELOCITY_X, PROJECTILE_VELOCITY_Y },
        Damage { PROJECTILE_DAMAGE },
        Hitbox { PROJECTILE_WIDTH, PROJECTILE_HEIGHT });
    Entity weapon = createWeapon(registry, Parent { enemy }, projectile);
    if (registry.has<Frequency>(weapon)) {
        auto& freq = registry.get<Frequency>(weapon);
        freq.frequency = 1.5;
        freq.lastTime = std::chrono::high_resolution_clock::now();
    }
    return enemy;
}

Entity createEnemy(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox, const Velocity& velocity)
{
    Entity enemy = registry.create_entity();
    registry.add<Position>(enemy, position);
    registry.add<PreviousPosition>(enemy, PreviousPosition { position.v.x, position.v.y });
    registry.add<Velocity>(enemy, velocity);
    registry.add<Health>(enemy, health);
    registry.add<Hitbox>(enemy, hitbox);
    registry.add<Dead>(enemy, Dead {});
    registry.add<EnemyTag>(enemy, EnemyTag {});
    // registry.emplace<Sprite>(enemy, 0, 50, 50);
    Entity projectile = factories::createProjectileTemplate(
        registry,
        Velocity { PROJECTILE_VELOCITY_X, PROJECTILE_VELOCITY_Y },
        Damage { PROJECTILE_DAMAGE },
        Hitbox { PROJECTILE_WIDTH, PROJECTILE_HEIGHT });
    Entity weapon = createWeapon(registry, Parent { enemy }, projectile);
    if (registry.has<Frequency>(weapon)) {
        auto& freq = registry.get<Frequency>(weapon);
        freq.frequency = 1.5;
        freq.lastTime = std::chrono::high_resolution_clock::now();
    }
    return enemy;
}

Entity createEnemyTemplate(Registry& registry, const Health& health, const Hitbox& hitbox, const Velocity& velocity)
{
    // Do NOT add Position/PreviousPosition (avoids movement) and no Weapon
    Entity enemy = registry.create_entity();
    registry.add<Velocity>(enemy, velocity);
    registry.add<Health>(enemy, health);
    registry.add<Hitbox>(enemy, hitbox);
    registry.add<Dead>(enemy, Dead {});
    registry.add<EnemyTag>(enemy, EnemyTag {});
    return enemy;
}
}
