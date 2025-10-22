/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteManager - Manages sprite creation and attachment to entities
*/

#include "SpriteManager.hpp"
#include "components/Hitbox.hpp"
#include "components/Sprite.hpp"
#include "components/SpriteFactory.hpp"
#include "components/SpriteUtils.hpp"
#include "components/Velocity.hpp"
#include <cmath>

void SpriteManager::addPlayerSprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor)
{
    (void)posX; // Unused parameter, kept for API compatibility
    (void)posY; // Unused parameter, kept for API compatibility

    if (!registry.has<Hitbox>(entity)) {
        return; // Cannot add sprite without hitbox
    }

    Hitbox& hitbox = registry.get<Hitbox>(entity);

    // Create sprite from registry
    Sprite sprite = SpriteFactory::createFromRegistry("player");

    // Calculate centered transform
    SpriteTransform transform = SpriteUtils::calculateCenteredTransform(sprite.size, hitbox, sizeFactor);
    sprite.scale = transform.scale;
    sprite.offset = transform.offset;

    registry.add<Sprite>(entity, sprite);
}

void SpriteManager::addEnemySprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor)
{
    (void)posX; // Unused parameter, kept for API compatibility
    (void)posY; // Unused parameter, kept for API compatibility

    if (!registry.has<Hitbox>(entity)) {
        return; // Cannot add sprite without hitbox
    }

    Hitbox& hitbox = registry.get<Hitbox>(entity);

    // Create sprite from registry
    Sprite sprite = SpriteFactory::createFromRegistry("enemy_bydo");

    // Calculate centered transform
    SpriteTransform transform = SpriteUtils::calculateCenteredTransform(sprite.size, hitbox, sizeFactor);
    sprite.scale = transform.scale;
    sprite.offset = transform.offset;

    registry.add<Sprite>(entity, sprite);
}

void SpriteManager::addPlatformSprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor)
{
    (void)posX; // Unused parameter, kept for API compatibility
    (void)posY; // Unused parameter, kept for API compatibility

    if (!registry.has<Hitbox>(entity)) {
        return; // Cannot add sprite without hitbox
    }

    Hitbox& hitbox = registry.get<Hitbox>(entity);

    // Create sprite from registry
    Sprite sprite = SpriteFactory::createFromRegistry("platform");

    // Calculate centered transform
    SpriteTransform transform = SpriteUtils::calculateCenteredTransform(sprite.size, hitbox, sizeFactor);
    sprite.scale = transform.scale;
    sprite.offset = transform.offset;

    registry.add<Sprite>(entity, sprite);
}

void SpriteManager::addProjectileSprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor)
{
    (void)posX;
    (void)posY;

    if (!registry.has<Hitbox>(entity)) {
        return; // Cannot add sprite without hitbox
    }

    Hitbox& hitbox = registry.get<Hitbox>(entity);

    // Create sprite from registry
    Sprite sprite = SpriteFactory::createFromRegistry("projectile_eye");

    // Calculate centered transform
    SpriteTransform transform = SpriteUtils::calculateCenteredTransform(sprite.size, hitbox, sizeFactor);
    sprite.scale = transform.scale;
    sprite.offset = transform.offset;

    // Calculate rotation based on velocity direction
    if (registry.has<Velocity>(entity)) {
        Velocity& velocity = registry.get<Velocity>(entity);
        sprite.rotation = atan2(velocity.v.y, velocity.v.x) * 180.0f / M_PI + 180.0f;
    }

    registry.add<Sprite>(entity, sprite);
}

void SpriteManager::addWeaponSprite(Registry& registry, Entity entity, float sizeFactor)
{
    Sprite sprite = SpriteFactory::createFromRegistry("weapon");
    
    sprite.scale = { sizeFactor, sizeFactor };
    
    float weaponWidth = sprite.size.x * sizeFactor;
    float weaponHeight = sprite.size.y * sizeFactor;
    
    sprite.offset = { -weaponWidth / 3.0f, -weaponHeight / 2.0f };
    
    sprite.rotationCenter = { weaponWidth / 3.0f, weaponHeight / 2.0f };

    registry.add<Sprite>(entity, sprite);
}
