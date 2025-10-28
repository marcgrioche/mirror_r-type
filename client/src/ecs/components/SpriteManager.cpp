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
#include "components/Velocity.hpp"
#include <cmath>

void SpriteManager::addPlayerSprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor)
{
    (void)posX;
    (void)posY;

    if (!registry.has<Hitbox>(entity)) {
        return; // Cannot add sprite without hitbox
    }

    Hitbox& hitbox = registry.get<Hitbox>(entity);

    const float SPRITE_WIDTH = 29.0f;
    const float SPRITE_HEIGHT = 29.0f;
    const int FRAME_NUMBER = 4;
    const float FRAME_DURATION = 0.15f;

    float scale_x = (hitbox.width * sizeFactor) / SPRITE_WIDTH;
    float scale_y = (hitbox.height * sizeFactor) / SPRITE_HEIGHT;

    float rendered_width = SPRITE_WIDTH * scale_x;
    float rendered_height = SPRITE_HEIGHT * scale_y;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    // Sprite sprite = SpriteFactory::createStaticSprite(
    //     "player_sprite.png", // texture ID
    //     0, 0, SPRITE_WIDTH, SPRITE_HEIGHT, // src rect (x, y, w, h) - full image
    //     scale_x, scale_y, // separate scales for exact hitbox matching
    //     offset_x, offset_y // offset to center on entity
    // );

    Sprite sprite = SpriteFactory::createAnimatedSprite(
        "player_sprite.png",
        SPRITE_WIDTH, SPRITE_HEIGHT,
        FRAME_NUMBER, FRAME_DURATION,
        scale_x, scale_y,
        offset_x, offset_y);

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

    // Bydo flying sprite sheet: 316x69 pixels total, 4 frames horizontally
    const int FRAME_WIDTH = 79; // 316/4 = 79 pixels per frame
    const int FRAME_HEIGHT = 69; // Full height of sprite sheet
    const int TOTAL_FRAMES = 4;
    const float FRAME_DURATION = 0.15f; // 150ms per frame for smooth animation

    // Apply size factor for visual scaling without changing hitbox
    float scale_x = (hitbox.width * sizeFactor) / FRAME_WIDTH;
    float scale_y = (hitbox.height * sizeFactor) / FRAME_HEIGHT;

    float rendered_width = FRAME_WIDTH * scale_x;
    float rendered_height = FRAME_HEIGHT * scale_y;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    Sprite sprite = SpriteFactory::createAnimatedSprite(
        "bydo_flying.png", // texture ID
        FRAME_WIDTH, FRAME_HEIGHT, // frame dimensions
        TOTAL_FRAMES, FRAME_DURATION, // animation parameters
        scale_x, scale_y, // separate scales for exact hitbox matching
        offset_x, offset_y // offset to center on entity
    );

    registry.add<Sprite>(entity, sprite);
}

void SpriteManager::addBossSprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor, const Level* level)
{
    (void)posX;
    (void)posY;

    if (!registry.has<Hitbox>(entity)) {
        return;
    }

    Hitbox& hitbox = registry.get<Hitbox>(entity);

    const int FRAME_WIDTH = level ? level->getBossFrameWidth() : 512;
    const int FRAME_HEIGHT = level ? level->getBossFrameHeight() : 512;
    const int TOTAL_FRAMES = level ? level->getBossFramesNb() : 3;
    const float FRAME_DURATION = level ? level->getBossFrameDuration() : 0.15;
    const std::string texture_id = level ? level->getBossIdlePath() : "heads_monster_idle.png";
    const std::string attack_id = level ? level->getBossAttackPaths()[0] : "heads_monster_idle.png";

    float scale_x = (hitbox.width * sizeFactor) / FRAME_WIDTH;
    float scale_y = (hitbox.height * sizeFactor) / FRAME_HEIGHT;

    float rendered_width = FRAME_WIDTH * scale_x;
    float rendered_height = FRAME_HEIGHT * scale_y;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    Sprite sprite = SpriteFactory::createAnimatedSprite(
        texture_id,
        FRAME_WIDTH, FRAME_HEIGHT,
        TOTAL_FRAMES, FRAME_DURATION,
        scale_x, scale_y,
        offset_x, offset_y, level ? level->getBossHealthStatesNumber() : 0,
        attack_id);

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

    const float SPRITE_WIDTH = 420.0f;
    const float SPRITE_HEIGHT = 190.0f;

    // Apply size factor for visual scaling without changing hitbox
    float scale_x = (hitbox.width * sizeFactor) / SPRITE_WIDTH;
    float scale_y = (hitbox.height * sizeFactor) / SPRITE_HEIGHT;

    float rendered_width = SPRITE_WIDTH * scale_x;
    float rendered_height = SPRITE_HEIGHT * scale_y;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    Sprite sprite = SpriteFactory::createStaticSprite(
        "WallOfFlesh.png", // texture ID
        0, 0, SPRITE_WIDTH, SPRITE_HEIGHT, // src rect (x, y, w, h) - full image
        scale_x, scale_y, // separate scales for exact hitbox matching
        offset_x, offset_y // offset to center on entity
    );

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

    // Déterminer si c'est un projectile joueur ou ennemi via velocity.x
    bool isPlayerProjectile = false;
    if (registry.has<Velocity>(entity)) {
        const Velocity& velocity = registry.get<Velocity>(entity);
        isPlayerProjectile = (velocity.v.x > 0);
    }

    Sprite sprite;
    
    if (isPlayerProjectile) {
        // Laser pour le joueur
        const float SPRITE_WIDTH = 249.0f;
        const float SPRITE_HEIGHT = 144.0f;

        float scale_x = (hitbox.width * sizeFactor) / SPRITE_WIDTH;
        float scale_y = (hitbox.height * sizeFactor) / SPRITE_HEIGHT;

        float rendered_width = SPRITE_WIDTH * scale_x;
        float rendered_height = SPRITE_HEIGHT * scale_y;
        float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
        float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

        sprite = SpriteFactory::createStaticSprite(
            "laser_bullet.png",
            0, 0, SPRITE_WIDTH, SPRITE_HEIGHT,
            scale_x, scale_y,
            offset_x, offset_y
        );
    } else {
        // Eye spritesheet pour les ennemis
        const int FRAME_WIDTH = 32;
        const int FRAME_HEIGHT = 24;
        const int TOTAL_FRAMES = 7;
        const float FRAME_DURATION = 0.1f;

        float scale_x = (hitbox.width * sizeFactor) / FRAME_WIDTH;
        float scale_y = (hitbox.height * sizeFactor) / FRAME_HEIGHT;

        float rendered_width = FRAME_WIDTH * scale_x;
        float rendered_height = FRAME_HEIGHT * scale_y;
        float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
        float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

        sprite = SpriteFactory::createAnimatedSprite(
            "eye_spritesheet.png",
            FRAME_WIDTH, FRAME_HEIGHT,
            TOTAL_FRAMES, FRAME_DURATION,
            scale_x, scale_y,
            offset_x, offset_y
        );
    }

    // Calculate rotation based on velocity direction
    if (registry.has<Velocity>(entity)) {
        const Velocity& velocity = registry.get<Velocity>(entity);
        float baseRotation = atan2(velocity.v.y, velocity.v.x) * 180.0f / M_PI;
        sprite.rotation = isPlayerProjectile ? baseRotation : (baseRotation + 180.0f);
    }

    registry.add<Sprite>(entity, sprite);
}

void SpriteManager::addWeaponSprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor)
{
    (void)posX;
    (void)posY;

    const float SPRITE_WIDTH = 66.0f;
    const float SPRITE_HEIGHT = 22.0f;

    float scale_x = sizeFactor;
    float scale_y = sizeFactor;

    float rendered_width = SPRITE_WIDTH * scale_x;
    float rendered_height = SPRITE_HEIGHT * scale_y;

    // Offset pour positionner l'arme devant le joueur
    float offset_x = -rendered_width / 3.0f;
    float offset_y = -rendered_height / 2.0f;

    Sprite sprite = SpriteFactory::createStaticSprite(
        "wp1.png", // texture ID
        0, 0, SPRITE_WIDTH, SPRITE_HEIGHT, // src rect
        scale_x, scale_y,
        offset_x, offset_y
    );

    registry.add<Sprite>(entity, sprite);
}
