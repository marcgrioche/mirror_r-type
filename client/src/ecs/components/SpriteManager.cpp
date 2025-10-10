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
    (void)posX; // Unused parameter, kept for API compatibility
    (void)posY; // Unused parameter, kept for API compatibility

    if (!registry.has<Hitbox>(entity)) {
        return; // Cannot add sprite without hitbox
    }

    Hitbox& hitbox = registry.get<Hitbox>(entity);

    const float SPRITE_WIDTH = 623.0f;
    const float SPRITE_HEIGHT = 623.0f;

    float scale_x = (hitbox.width * sizeFactor) / SPRITE_WIDTH;
    float scale_y = (hitbox.height * sizeFactor) / SPRITE_HEIGHT;

    float rendered_width = SPRITE_WIDTH * scale_x;
    float rendered_height = SPRITE_HEIGHT * scale_y;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    Sprite sprite = SpriteFactory::createStaticSprite(
        "player_sprite.png", // texture ID
        0, 0, SPRITE_WIDTH, SPRITE_HEIGHT, // src rect (x, y, w, h) - full image
        scale_x, scale_y, // separate scales for exact hitbox matching
        offset_x, offset_y // offset to center on entity
    );

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

    // Eye spritesheet: 224x24 pixels total, 7 frames, each 32x24 pixels
    const int FRAME_WIDTH = 32;
    const int FRAME_HEIGHT = 24;
    const int TOTAL_FRAMES = 7;
    const float FRAME_DURATION = 0.1f; // 100ms per frame

    // Apply size factor for visual scaling without changing hitbox
    float scale_x = (hitbox.width * sizeFactor) / FRAME_WIDTH;
    float scale_y = (hitbox.height * sizeFactor) / FRAME_HEIGHT;

    float rendered_width = FRAME_WIDTH * scale_x;
    float rendered_height = FRAME_HEIGHT * scale_y;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    Sprite sprite = SpriteFactory::createAnimatedSprite(
        "eye_spritesheet.png", // texture ID
        FRAME_WIDTH, FRAME_HEIGHT, // frame dimensions
        TOTAL_FRAMES, FRAME_DURATION, // animation parameters
        scale_x, scale_y, // separate scales for exact hitbox matching
        offset_x, offset_y // offset to center on entity
    );

    // Calculate rotation based on velocity direction
    if (registry.has<Velocity>(entity)) {
        Velocity& velocity = registry.get<Velocity>(entity);
        sprite.rotation = atan2(velocity.dy, velocity.dx) * 180.0f / M_PI + 180.0f;
    }

    registry.add<Sprite>(entity, sprite);
}
