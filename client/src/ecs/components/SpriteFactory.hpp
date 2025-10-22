/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteFactory - Utility functions for creating sprites
*/

#pragma once
#include "ResourceManager.hpp"
#include "Sprite.hpp"
#include <string>

class SpriteFactory {
public:
    static Sprite createFromRegistry(const std::string& spriteKey,
        float scaleX = 1.0f, float scaleY = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f);

    static Sprite createStaticSprite(const std::string& textureId,
        float width, float height,
        float scaleX = 1.0f, float scaleY = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f);

    static Sprite createAnimatedSprite(const std::string& textureId,
        float frameWidth, float frameHeight,
        int totalFrames, float frameDuration = 0.1f,
        float scaleX = 1.0f, float scaleY = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f);
};
