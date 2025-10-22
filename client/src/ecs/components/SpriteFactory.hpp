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
    /**
     * @brief Creates a sprite from the sprite registry
     * @param spriteKey Key name in the sprite registry (e.g., "player", "enemy_bydo")
     * @param scaleX Horizontal scaling factor (default 1.0)
     * @param scaleY Vertical scaling factor (default 1.0)
     * @param offsetX X offset for sprite positioning (default 0)
     * @param offsetY Y offset for sprite positioning (default 0)
     * @return Configured Sprite object
     */
    static Sprite createFromRegistry(const std::string& spriteKey,
        float scaleX = 1.0f, float scaleY = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f);

    /**
     * @brief Creates a static (non-animated) sprite
     * @param textureId Identifier for the texture resource
     * @param width Sprite width
     * @param height Sprite height
     * @param scaleX Horizontal scaling factor
     * @param scaleY Vertical scaling factor
     * @param offsetX X offset for sprite positioning
     * @param offsetY Y offset for sprite positioning
     * @return Configured Sprite object
     */
    static Sprite createStaticSprite(const std::string& textureId,
        float width, float height,
        float scaleX = 1.0f, float scaleY = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f);

    /**
     * @brief Creates an animated sprite with multiple frames
     * @param textureId Identifier for the texture resource
     * @param frameWidth Width of each animation frame
     * @param frameHeight Height of each animation frame
     * @param totalFrames Total number of frames in the animation
     * @param frameDuration Time in seconds each frame is displayed
     * @param scaleX Horizontal scaling factor
     * @param scaleY Vertical scaling factor
     * @param offsetX X offset for sprite positioning
     * @param offsetY Y offset for sprite positioning
     * @return Configured Sprite object
     */
    static Sprite createAnimatedSprite(const std::string& textureId,
        float frameWidth, float frameHeight,
        int totalFrames, float frameDuration = 0.1f,
        float scaleX = 1.0f, float scaleY = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f);
};
