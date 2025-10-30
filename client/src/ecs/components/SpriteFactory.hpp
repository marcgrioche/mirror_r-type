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
     * @brief Creates a static (non-animated) sprite
     * @param textureId Identifier for the texture resource
     * @param x Source rectangle X position in texture
     * @param y Source rectangle Y position in texture
     * @param width Source rectangle width
     * @param height Source rectangle height
     * @param scaleX Horizontal scaling factor
     * @param scaleY Vertical scaling factor
     * @param offsetX X offset for sprite positioning
     * @param offsetY Y offset for sprite positioning
     * @return Configured Sprite object
     */
    static Sprite createStaticSprite(const std::string& textureId,
        int x, int y, int width, int height,
        float scaleX = 1.0f, float scaleY = 1.0f, int offsetX = 0, int offsetY = 0);

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
        int frameWidth, int frameHeight,
        int totalFrames, float frameDuration = 0.1f,
        float scaleX = 1.0f, float scaleY = 1.0f, int offsetX = 0, int offsetY = 0, int health_state_nb = 0, const std::string& attackId = "");

    /**
     * @brief Creates a custom sprite with full control over source and destination rectangles
     * @param textureId Identifier for the texture resource
     * @param srcRect Source rectangle defining the area to render from texture
     * @param dstRect Destination rectangle defining where to render on screen
     * @param scale Overall scaling factor
     * @param rotation Rotation angle in degrees
     * @param offsetX X offset for sprite positioning
     * @param offsetY Y offset for sprite positioning
     * @return Configured Sprite object
     */
    static Sprite createCustomSprite(const std::string& textureId,
        SDL_Rect srcRect, SDL_Rect dstRect,
        float scale = 1.0f, float rotation = 0.0f,
        int offsetX = 0, int offsetY = 0);
};
