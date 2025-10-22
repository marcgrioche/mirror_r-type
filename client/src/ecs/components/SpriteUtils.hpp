/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteUtils - Utility functions for sprite calculations
*/

#pragma once
#include "Sprite.hpp"
#include "components/Hitbox.hpp"

struct SpriteTransform {
    Vector2 scale;
    Vector2 offset;
};

namespace SpriteUtils {

SpriteTransform calculateCenteredTransform(
    const Vector2& spriteSize,
    const Hitbox& hitbox,
    float sizeFactor = 1.0f);

SDL_Rect calculateSourceRect(const Sprite& sprite);

SDL_Rect calculateDestRect(const Sprite& sprite, const Vector2& position);

} // namespace SpriteUtils
