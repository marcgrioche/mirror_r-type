/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** enemy
*/
#include "CreateBoss.hpp"
#include "../enemies/CreateEnemy.hpp"
#include "levels/Level.hpp"

namespace factories {
Entity createBoss(Registry& registry, const Level* level)
{
    float bossHealth = level ? level->getBossHealth() : BOSS_HEALTH;
    float width = level ? level->getBossWidth() : BOSS_WIDTH;
    float height = level ? level->getBossHeight() : BOSS_HEIGHT;
    std::cout << "heigh = " << height << " width = " << width << std::endl;
    std::cout << "HEIGHT = " << BOSS_HEIGHT << " WIDTH = " << BOSS_WIDTH << std::endl;
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float posX = level ? (SCREEN_WIDTH - width * level->getPosXFactor()) : SCREEN_WIDTH - BOSS_WIDTH;
    float posY = 0.0f;

    Entity boss = registry.create_entity();
    registry.emplace<Position>(boss, posX, posY);
    registry.emplace<PreviousPosition>(boss, posX, posY);
    registry.emplace<Velocity>(boss, velocityX, velocityY);
    registry.emplace<Health>(boss, static_cast<int>(bossHealth));
    registry.emplace<Hitbox>(boss, width, height, 0.0f, 0.0f);
    registry.emplace<Dead>(boss);
    registry.emplace<EnemyTag>(boss);
    registry.emplace<BossTag>(boss);

    // Weapon for shooting projectiles
    float bossProjectileSpeed = level ? level->getBossProjectileSpeed() : PROJECTILE_VELOCITY_X;
    int bossProjectileDamage = level ? level->getBossProjectileDamage() : PROJECTILE_DAMAGE;
    float bossProjectileWidth = level ? level->getBossProjectileWidth() : PROJECTILE_WIDTH;
    float bossProjectileHeight = level ? level->getBossProjectileHeight() : PROJECTILE_HEIGHT;
    Entity projectile = factories::createProjectileTemplate(
        registry,
        Velocity { bossProjectileSpeed, PROJECTILE_VELOCITY_Y },
        Damage { static_cast<float>(bossProjectileDamage) },
        Hitbox { bossProjectileWidth, bossProjectileHeight });
    Entity shootWeapon = createWeapon(registry, Parent { boss }, projectile);
    if (registry.has<Frequency>(shootWeapon)) {
        auto& freq = registry.get<Frequency>(shootWeapon);
        freq.frequency = level ? level->getBossShootFrequency() : 2.0f;
        freq.lastTime = std::chrono::high_resolution_clock::now();
    }

    // Weapon for spawning enemies
    Entity enemyTemplate = factories::createEnemyTemplate(
        registry,
        Health { level ? level->getEnemyHealth() : ENEMY_HEALTH },
        Hitbox { level ? level->getEnemyWidth() : ENEMY_WIDTH, level ? level->getEnemyHeight() : ENEMY_HEIGHT },
        Velocity { level ? level->getEnemyVelocityX() : ENEMY_VELOCITY_X, level ? level->getEnemyVelocityY() : ENEMY_VELOCITY_Y });
    Entity spawnWeapon = createWeapon(registry, Parent { boss }, enemyTemplate);
    if (registry.has<Frequency>(spawnWeapon)) {
        auto& freq = registry.get<Frequency>(spawnWeapon);
        freq.frequency = level ? level->getBossSpawnEnemyFrequency() : 5.0f;
        freq.lastTime = std::chrono::high_resolution_clock::now();
    }

    return boss;
}

Entity createBoss(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox, const Velocity& velocity)
{
    Entity boss = createEnemy(registry, position, health, hitbox, velocity);

    registry.emplace<BossTag>(boss);

    Entity enemy = factories::createEnemyTemplate(
        registry,
        Health { ENEMY_HEALTH },
        Hitbox { ENEMY_WIDTH, ENEMY_HEIGHT },
        Velocity { ENEMY_VELOCITY_X, ENEMY_VELOCITY_Y });
    Entity weapon = createWeapon(registry, Parent { boss }, enemy);
    if (registry.has<Frequency>(weapon)) {
        auto& freq = registry.get<Frequency>(weapon);
        freq.frequency = 3;
        freq.lastTime = std::chrono::high_resolution_clock::now();
    }
    return boss;
}
}
