/*
** SpriteRender.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 15 5:45:25 PM 2025 jojo
** Last update Thu Oct 15 10:23:26 PM 2025 jojo
*/

#include "SpriteRender.hpp"
#include "ResourceManager.hpp"
#include <iostream>

void drawSprite(GraphicsManager& gfx, Registry& registry, Entity sprite)
{
    SDL_Renderer* renderer = gfx.getRenderer();
    auto& resourceManager = ResourceManager::getInstance();

    if (!registry.has<Sprite>(sprite))
        return;

    const Sprite& spr = registry.get<Sprite>(sprite);

    // Source rect (may be adjusted per-frame for animated sprites)
    SDL_Rect src = spr.srcRect;
    if (src.w == 0 || src.h == 0) {
        // fallback to frame values
        src = { 0, 0, spr.frame_width, spr.frame_height };
    }

    // If animated, compute the correct frame X offset
    if (spr.total_frames > 1) {
        int frame = spr.current_frameX;
        if (frame < 0)
            frame = 0;
        if (frame >= spr.total_frames)
            frame = spr.total_frames - 1;
        src.x = spr.srcRect.x + frame * spr.frame_width;
        src.y = spr.srcRect.y; // assume frames are laid out horizontally
    }

    const SDL_Rect* srcPtr = (src.w > 0 && src.h > 0) ? &src : nullptr;

    // Destination rect (use sprite dstRect but compute size if zero)
    SDL_Rect dst = spr.dstRect;
    if (dst.w == 0 || dst.h == 0) {
        dst.w = static_cast<int>(spr.frame_width * (spr.scale_x > 0.0f ? spr.scale_x : spr.scale));
        dst.h = static_cast<int>(spr.frame_height * (spr.scale_y > 0.0f ? spr.scale_y : spr.scale));
    }

    // Apply Position if present (position is the top-left of dst)
    if (registry.has<Position>(sprite)) {
        const Position& pos = registry.get<Position>(sprite);
        dst.x = static_cast<int>(pos.v.x) + spr.offset_x;
        dst.y = static_cast<int>(pos.v.y) + spr.offset_y;
    }

    SDL_Texture* tex = resourceManager.getTexture(spr.texture_id);
    if (!tex) {
        std::cout << "SpriteRender: texture not found -> " << spr.texture_id << std::endl;
        return;
    }

    // rotation if present on Sprite, otherwise 0
    double angle = 0.0;
#ifdef HAS_SPRITE_ROTATION
    angle = spr.rotation;
#elif defined(SPRITE_HAS_ROTATION)
    angle = spr.rotation;
#endif

    SDL_RenderCopyEx(renderer, tex, srcPtr, &dst, angle, nullptr, SDL_FLIP_NONE);
}