/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteFactory - Utility functions for creating sprites
*/

#include "SpriteFactory.hpp"
#include "SpriteRegistry.hpp"
#include <iostream>

Sprite SpriteFactory::createFromRegistry(const std::string& spriteKey,
    float scaleX, float scaleY, float offsetX, float offsetY)
{
    const SpriteConfig* config = SpriteRegistry::get(spriteKey);

    if (!config) {
        std::cerr << "Warning: Sprite '" << spriteKey << "' not found in registry" << std::endl;
        Sprite sprite;
        sprite.texture_id = spriteKey;
        sprite.size = { 32.0f, 32.0f };
        sprite.scale = { scaleX, scaleY };
        sprite.offset = { offsetX, offsetY };
        return sprite;
    }

    Sprite sprite;
    sprite.texture_id = config->textureId;
    sprite.scale = { scaleX, scaleY };
    sprite.offset = { offsetX, offsetY };
    sprite.rotation = 0.0f;

    if (config->animated) {
        sprite.size = { config->frameWidth, config->frameHeight };
        sprite.total_frames = config->frames;
        sprite.frame_duration = config->frameDuration;
    } else {
        sprite.size = { config->width, config->height };
        sprite.total_frames = 1;
    }

    sprite.current_frame = 0;
    sprite.elapsed_time = 0.0f;

    return sprite;
}

Sprite SpriteFactory::createStaticSprite(const std::string& textureId,
    float width, float height,
    float scaleX, float scaleY, float offsetX, float offsetY)
{
    Sprite sprite;
    sprite.texture_id = textureId;
    sprite.size = { width, height };
    sprite.scale = { scaleX, scaleY };
    sprite.offset = { offsetX, offsetY };
    sprite.rotation = 0.0f;
    sprite.total_frames = 1;
    sprite.current_frame = 0;
    sprite.frame_duration = 0.0f;
    sprite.elapsed_time = 0.0f;

    return sprite;
}

Sprite SpriteFactory::createAnimatedSprite(const std::string& textureId,
    float frameWidth, float frameHeight,
    int totalFrames, float frameDuration,
    float scaleX, float scaleY, float offsetX, float offsetY)
{
    Sprite sprite;
    sprite.texture_id = textureId;
    sprite.size = { frameWidth, frameHeight };
    sprite.scale = { scaleX, scaleY };
    sprite.offset = { offsetX, offsetY };
    sprite.rotation = 0.0f;
    sprite.total_frames = totalFrames;
    sprite.current_frame = 0;
    sprite.frame_duration = frameDuration;
    sprite.elapsed_time = 0.0f;

    return sprite;
}
