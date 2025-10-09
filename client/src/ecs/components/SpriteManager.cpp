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

void SpriteManager::addPlayerSprite(Registry& registry, Entity entity, float posX, float posY)
{
    (void)posX; // Unused parameter, kept for API compatibility
    (void)posY; // Unused parameter, kept for API compatibility

    if (!registry.has<Hitbox>(entity)) {
        return; // Cannot add sprite without hitbox
    }

    Hitbox& hitbox = registry.get<Hitbox>(entity);

    const float SPRITE_WIDTH = 623.0f;
    const float SPRITE_HEIGHT = 623.0f;

    float scale = (hitbox.width < hitbox.height) ? hitbox.width / SPRITE_WIDTH : hitbox.height / SPRITE_HEIGHT;

    float rendered_width = SPRITE_WIDTH * scale;
    float rendered_height = SPRITE_HEIGHT * scale;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    Sprite sprite = SpriteFactory::createStaticSprite(
        "player_sprite.png", // texture ID
        0, 0, SPRITE_WIDTH, SPRITE_HEIGHT, // src rect (x, y, w, h) - full image
        scale, // scale to fit hitbox
        offset_x, offset_y // offset to center on entity
    );

    registry.add<Sprite>(entity, sprite);
}

void SpriteManager::addProjectileSprite(Registry& registry, Entity entity, float posX, float posY)
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

    float scale = (hitbox.width < hitbox.height) ? hitbox.width / FRAME_WIDTH : hitbox.height / FRAME_HEIGHT;

    float rendered_width = FRAME_WIDTH * scale;
    float rendered_height = FRAME_HEIGHT * scale;
    float offset_x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    float offset_y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    Sprite sprite = SpriteFactory::createAnimatedSprite(
        "eye_spritesheet.png", // texture ID
        FRAME_WIDTH, FRAME_HEIGHT, // frame dimensions
        TOTAL_FRAMES, FRAME_DURATION, // animation parameters
        scale, // scale to fit hitbox
        offset_x, offset_y // offset to center on entity
    );

    // Calculate rotation based on velocity direction
    if (registry.has<Velocity>(entity)) {
        Velocity& velocity = registry.get<Velocity>(entity);
        sprite.rotation = atan2(velocity.dy, velocity.dx) * 180.0f / M_PI + 180.0f;
    }

    registry.add<Sprite>(entity, sprite);
}
