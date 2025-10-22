/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteUtils - Utility functions for sprite calculations
*/

#include "SpriteUtils.hpp"

namespace SpriteUtils {

SpriteTransform calculateCenteredTransform(
    const Vector2& spriteSize,
    const Hitbox& hitbox,
    float sizeFactor)
{
    SpriteTransform transform;

    // Calculate scale factors to fit sprite to hitbox
    transform.scale.x = (hitbox.width * sizeFactor) / spriteSize.x;
    transform.scale.y = (hitbox.height * sizeFactor) / spriteSize.y;

    // Calculate rendered dimensions
    float rendered_width = spriteSize.x * transform.scale.x;
    float rendered_height = spriteSize.y * transform.scale.y;

    // Calculate offset to center sprite on hitbox
    transform.offset.x = -(rendered_width / 2.0f) + (hitbox.width / 2.0f);
    transform.offset.y = -(rendered_height / 2.0f) + (hitbox.height / 2.0f);

    return transform;
}

SDL_Rect calculateSourceRect(const Sprite& sprite)
{
    SDL_Rect srcRect;

    if (sprite.total_frames > 1) {
        // Animated sprite - calculate frame position
        int frame = sprite.current_frame;
        if (frame < 0) frame = 0;
        if (frame >= sprite.total_frames) frame = sprite.total_frames - 1;

        srcRect.x = frame * static_cast<int>(sprite.size.x);
        srcRect.y = 0; // Assumes horizontal sprite sheet
    } else {
        // Static sprite - use full texture
        srcRect.x = 0;
        srcRect.y = 0;
    }

    srcRect.w = static_cast<int>(sprite.size.x);
    srcRect.h = static_cast<int>(sprite.size.y);

    return srcRect;
}

SDL_Rect calculateDestRect(const Sprite& sprite, const Vector2& position)
{
    SDL_Rect dstRect;

    dstRect.x = static_cast<int>(position.x + sprite.offset.x);
    dstRect.y = static_cast<int>(position.y + sprite.offset.y);
    dstRect.w = static_cast<int>(sprite.size.x * sprite.scale.x);
    dstRect.h = static_cast<int>(sprite.size.y * sprite.scale.y);

    return dstRect;
}

} // namespace SpriteUtils
