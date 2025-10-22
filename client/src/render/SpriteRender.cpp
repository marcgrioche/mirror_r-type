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
#include "components/SpriteUtils.hpp"
#include <iostream>

void drawSprite(GraphicsManager& gfx, Registry& registry, Entity sprite)
{
    SDL_Renderer* renderer = gfx.getRenderer();
    auto& resourceManager = ResourceManager::getInstance();

    if (!registry.has<Sprite>(sprite))
        return;

    const Sprite& spr = registry.get<Sprite>(sprite);

    // Calculate source and destination rectangles using SpriteUtils
    SDL_Rect srcRect = SpriteUtils::calculateSourceRect(spr);

    SDL_Rect dstRect;
    if (registry.has<Position>(sprite)) {
        const Position& pos = registry.get<Position>(sprite);
        dstRect = SpriteUtils::calculateDestRect(spr, pos.v);
    } else {
        // If no position, render at origin
        dstRect = SpriteUtils::calculateDestRect(spr, { 0.0f, 0.0f });
    }

    SDL_Texture* tex = resourceManager.getTexture(spr.texture_id);
    if (!tex) {
        std::cout << "SpriteRender: texture not found -> " << spr.texture_id << std::endl;
        return;
    }

    SDL_Point* center = nullptr;
    SDL_Point rotCenter;
    if (spr.rotationCenter.x >= 0.0f && spr.rotationCenter.y >= 0.0f) {
        rotCenter.x = static_cast<int>(spr.rotationCenter.x);
        rotCenter.y = static_cast<int>(spr.rotationCenter.y);
        center = &rotCenter;
    }

    SDL_RenderCopyEx(renderer, tex, &srcRect, &dstRect, spr.rotation, center, SDL_FLIP_NONE);
}