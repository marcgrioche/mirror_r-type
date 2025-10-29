/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteManager - Manages sprite creation and attachment to entities
*/

#include "SpriteManager.hpp"
#include "components/Hitbox.hpp"
#include "components/PlayerSyncState.hpp"
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
        "player_idle_without_head.png",
        SPRITE_WIDTH, SPRITE_HEIGHT,
        FRAME_NUMBER, FRAME_DURATION,
        scale_x, scale_y,
        offset_x, offset_y);

    registry.add<Sprite>(entity, sprite);

    if (!registry.has<PlayerSyncState>(entity)) {
        registry.add<PlayerSyncState>(entity, PlayerSyncState { PlayerMovementState::IDLE, FacingDirection::RIGHT });
    }
}

void SpriteManager::addEnemySprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor, const Level* level)
{
    (void)posX; // Unused parameter, kept for API compatibility
    (void)posY; // Unused parameter, kept for API compatibility

    if (!registry.has<Hitbox>(entity)) {
        return; // Cannot add sprite without hitbox
    }

    Hitbox& hitbox = registry.get<Hitbox>(entity);

    // Use level-provided animation params (fallback to defaults if level is null)
    const int FRAME_WIDTH = level ? level->getEnemyFrameWidth() : 79;
    const int FRAME_HEIGHT = level ? level->getEnemyFrameHeight() : 69;
    const int TOTAL_FRAMES = level ? level->getEnemyFramesNb() : 4;
    const float FRAME_DURATION = level ? level->getEnemyFrameDuration() : 0.15f;

    // Apply size factor for visual scaling without changing hitbox
    float scale_x = (hitbox.width * sizeFactor) / FRAME_WIDTH;
    float scale_y = (hitbox.height * sizeFactor) / FRAME_HEIGHT;

    float rendered_width = FRAME_WIDTH * scale_x;
    float rendered_height = FRAME_HEIGHT * scale_y;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    // Use level-specific enemy sprite id if provided; fallback to default bydo
    const std::string enemyTextureId = (level && !level->getEnemySpriteId().empty()) ? level->getEnemySpriteId() : std::string("bydo_flying.png");

    Sprite sprite = SpriteFactory::createAnimatedSprite(
        enemyTextureId,
        FRAME_WIDTH, FRAME_HEIGHT,
        TOTAL_FRAMES, FRAME_DURATION,
        scale_x, scale_y,
        offset_x, offset_y
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

    // If texture was downscaled at load time, adjust frame dimensions accordingly
    auto& resourceManager = ResourceManager::getInstance();
    float texScale = resourceManager.getTextureScale(texture_id);
    int effectiveFrameW = static_cast<int>(FRAME_WIDTH * texScale);
    int effectiveFrameH = static_cast<int>(FRAME_HEIGHT * texScale);

    float scale_x = (hitbox.width * sizeFactor) / effectiveFrameW;
    float scale_y = (hitbox.height * sizeFactor) / effectiveFrameH;

    float rendered_width = FRAME_WIDTH * scale_x;
    float rendered_height = FRAME_HEIGHT * scale_y;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    Sprite sprite = SpriteFactory::createAnimatedSprite(
    texture_id,
    effectiveFrameW, effectiveFrameH,
        TOTAL_FRAMES, FRAME_DURATION,
        scale_x, scale_y,
        offset_x, offset_y, level ? level->getBossHealthStatesNumber() : 0,
        attack_id);

    registry.add<Sprite>(entity, sprite);
}

void SpriteManager::addPlatformSprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor, const Level* level)
{
    (void)posX;
    (void)posY;

    if (!registry.has<Hitbox>(entity)) {
        return;
    }

    Hitbox& hitbox = registry.get<Hitbox>(entity);

    const float SPRITE_WIDTH = level ? level->getPlatformWidth() : 1.0;
    const float SPRITE_HEIGHT = level ? level->getPlatformHeight() : 1.0;

    float scale_x = (hitbox.width * sizeFactor) / SPRITE_WIDTH;
    float scale_y = (hitbox.height * sizeFactor) / SPRITE_HEIGHT;

    float rendered_width = SPRITE_WIDTH * scale_x;
    float rendered_height = SPRITE_HEIGHT * scale_y;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    Sprite sprite = SpriteFactory::createStaticSprite(
        level ? level->getPlatformSpritePath() : "",
        0, 0, SPRITE_WIDTH, SPRITE_HEIGHT,
        scale_x, scale_y,
        offset_x, offset_y
    );

    registry.add<Sprite>(entity, sprite);
}

void SpriteManager::addProjectileSprite(Registry& registry, Entity entity, float posX, float posY, float sizeFactor, const Level* level)
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
            offset_x, offset_y);
    } else {
        // Enemy projectile uses level-configured animation params
        const int FRAME_WIDTH = level ? level->getProjectileFrameWidth() : 32;
        const int FRAME_HEIGHT = level ? level->getProjectileFrameHeight() : 24;
        const int TOTAL_FRAMES = level ? level->getProjectileFramesNb() : 7;
        const float FRAME_DURATION = level ? level->getProjectileFrameDuration() : 0.1f;

        float scale_x = (hitbox.width * sizeFactor) / FRAME_WIDTH;
        float scale_y = (hitbox.height * sizeFactor) / FRAME_HEIGHT;

        float rendered_width = FRAME_WIDTH * scale_x;
        float rendered_height = FRAME_HEIGHT * scale_y;
        float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
        float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

        // Use level-specific enemy projectile sprite id if provided; fallback to legacy eye spritesheet
        const std::string enemyProjTextureId = (level && !level->getEnemyProjectileSpriteId().empty()) ? level->getEnemyProjectileSpriteId() : std::string("eye_spritesheet.png");

        sprite = SpriteFactory::createAnimatedSprite(
            enemyProjTextureId,
            FRAME_WIDTH, FRAME_HEIGHT,
            TOTAL_FRAMES, FRAME_DURATION,
            scale_x, scale_y,
            offset_x, offset_y);
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
        offset_x, offset_y);

    registry.add<Sprite>(entity, sprite);
}
