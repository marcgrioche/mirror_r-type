/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteUtils - Utility functions for sprite calculations
*/

#pragma once
#include "Sprite.hpp"
#include "components/Hitbox.hpp"

/**
 * @brief Transform data for centered sprite positioning
 */
struct SpriteTransform {
    Vector2 scale;   // X/Y scale factors
    Vector2 offset;  // X/Y offset to center sprite on hitbox
};

namespace SpriteUtils {

/**
 * @brief Calculates scale and offset to center a sprite on a hitbox
 * @param spriteSize Original size of the sprite (width, height)
 * @param hitbox The hitbox to center the sprite on
 * @param sizeFactor Additional scaling factor (1.0 = match hitbox exactly)
 * @return Transform containing scale and offset values
 */
SpriteTransform calculateCenteredTransform(
    const Vector2& spriteSize,
    const Hitbox& hitbox,
    float sizeFactor = 1.0f);

/**
 * @brief Calculates the source rectangle for animated sprite frames
 * @param sprite The sprite with animation data
 * @return SDL_Rect defining the source area in the spritesheet
 */
SDL_Rect calculateSourceRect(const Sprite& sprite);

/**
 * @brief Calculates the destination rectangle for rendering
 * @param sprite The sprite with size and scale data
 * @param position Entity position (x, y)
 * @return SDL_Rect defining where to render on screen
 */
SDL_Rect calculateDestRect(const Sprite& sprite, const Vector2& position);

} // namespace SpriteUtils
