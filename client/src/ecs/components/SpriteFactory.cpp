/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteFactory - Utility functions for creating sprites
*/

#include "SpriteFactory.hpp"

Sprite SpriteFactory::createStaticSprite(const std::string& textureId,
    int x, int y, int width, int height,
    float scaleX, float scaleY, int offsetX, int offsetY)
{
    Sprite sprite;
    sprite.texture_id = textureId;
    sprite.srcRect = { x, y, width, height };
    sprite.dstRect = { 0, 0, width, height };
    sprite.offset_x = offsetX;
    sprite.offset_y = offsetY;
    sprite.scale = scaleX;
    sprite.scale_x = scaleX;
    sprite.scale_y = scaleY;
    sprite.rotation = 0.0f;

    sprite.total_frames = 1;
    sprite.current_frameX = 0;
    sprite.current_frameY = 0;
    sprite.frame_duration = 0.0f;
    sprite.elapsed_time = 0.0f;
    sprite.frame_width = width;
    sprite.frame_height = height;

    return sprite;
}

Sprite SpriteFactory::createAnimatedSprite(const std::string& textureId,
    int frameWidth, int frameHeight,
    int totalFrames, float frameDuration,
    float scaleX, float scaleY, int offsetX, int offsetY)
{
    Sprite sprite;
    sprite.texture_id = textureId;
    sprite.srcRect = { 0, 0, frameWidth, frameHeight };
    sprite.dstRect = { 0, 0, frameWidth, frameHeight };
    sprite.offset_x = offsetX;
    sprite.offset_y = offsetY;
    sprite.scale = scaleX;
    sprite.scale_x = scaleX;
    sprite.scale_y = scaleY; // Independent Y scaling
    sprite.rotation = 0.0f;

    sprite.total_frames = totalFrames;
    sprite.current_frameX = 0;
    sprite.current_frameY = 0;
    sprite.frame_duration = frameDuration;
    sprite.elapsed_time = 0.0f;
    sprite.frame_width = frameWidth;
    sprite.frame_height = frameHeight;

    return sprite;
}

Sprite SpriteFactory::createCustomSprite(const std::string& textureId,
    SDL_Rect srcRect, SDL_Rect dstRect,
    float scale, float rotation,
    int offsetX, int offsetY)
{
    Sprite sprite;
    sprite.texture_id = textureId;
    sprite.srcRect = srcRect;
    sprite.dstRect = dstRect;
    sprite.offset_x = offsetX;
    sprite.offset_y = offsetY;
    sprite.scale = scale;
    sprite.rotation = rotation;

    sprite.total_frames = 1;
    sprite.current_frameX = 0;
    sprite.current_frameY = 0;
    sprite.frame_duration = 0.0f;
    sprite.elapsed_time = 0.0f;
    sprite.frame_width = srcRect.w;
    sprite.frame_height = srcRect.h;

    return sprite;
}
