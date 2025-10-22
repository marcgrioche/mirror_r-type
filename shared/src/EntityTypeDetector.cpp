/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Type Detector Implementation
*/

#include "../include/EntityTypeDetector.hpp"
#include "../src/ecs/components/Tags.hpp"
#include <stdexcept>

EntityType EntityTypeDetector::detectEntityType(const Registry& registry, Entity entity)
{
    if (isBoss(registry, entity))
        return EntityType::BOSS;
    if (isPlayer(registry, entity))
        return EntityType::PLAYER;
    if (isProjectile(registry, entity))
        return EntityType::PROJECTILE;
    if (isPlatform(registry, entity))
        return EntityType::PLATFORM;
    if (isPowerUp(registry, entity))
        return EntityType::POWERUP;
    if (isEnemy(registry, entity))
        return EntityType::ENEMY;

    throw std::runtime_error("Unable to determine entity type - no recognized tags found");
}

bool EntityTypeDetector::isPlayer(const Registry& registry, Entity entity)
{
    return registry.has<PlayerTag>(entity);
}

bool EntityTypeDetector::isProjectile(const Registry& registry, Entity entity)
{
    return registry.has<ProjectileTag>(entity);
}

bool EntityTypeDetector::isPlatform(const Registry& registry, Entity entity)
{
    return registry.has<PlatformTag>(entity);
}

bool EntityTypeDetector::isEnemy(const Registry& registry, Entity entity)
{
    return registry.has<EnemyTag>(entity);
}

bool EntityTypeDetector::isBoss(const Registry& registry, Entity entity)
{
    return registry.has<BossTag>(entity);
}

bool EntityTypeDetector::isPowerUp(const Registry& registry, Entity entity)
{
    return registry.has<PowerUpTag>(entity);
}
