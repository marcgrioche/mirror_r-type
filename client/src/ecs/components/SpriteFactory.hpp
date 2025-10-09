/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteFactory - Utility functions for creating sprites
*/

#pragma once
#include "Sprite.hpp"
#include <string>

class SpriteFactory {
public:
    static Sprite createStaticSprite(const std::string& textureId,
        int x, int y, int width, int height,
        float scaleX = 1.0f, float scaleY = 1.0f, int offsetX = 0, int offsetY = 0);

    static Sprite createAnimatedSprite(const std::string& textureId,
        int frameWidth, int frameHeight,
        int totalFrames, float frameDuration = 0.1f,
        float scaleX = 1.0f, float scaleY = 1.0f, int offsetX = 0, int offsetY = 0);

    static Sprite createCustomSprite(const std::string& textureId,
        SDL_Rect srcRect, SDL_Rect dstRect,
        float scale = 1.0f, float rotation = 0.0f,
        int offsetX = 0, int offsetY = 0);
};
