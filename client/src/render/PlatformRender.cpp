/*
** PlatformRender.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 6 10:07:13 AM 2025 jojo
** Last update Tue Oct 6 10:39:15 AM 2025 jojo
*/

#include "PlatformRender.hpp"

void drawPlatform(GraphicsManager& gfx, Registry& registry, Entity entity)
{
    SDL_Renderer* renderer = gfx.getRenderer();

    if (!registry.has<PlatformTag>(entity))
        return;

    const Position& pos = registry.get<Position>(entity);
    const Hitbox& hitbox = registry.get<Hitbox>(entity);

    SDL_Color bgColor = { 255, 255, 255, 255 };

    SDL_Rect PlatformRect = {
        static_cast<int>(pos.x + hitbox.offset_x),
        static_cast<int>(pos.y + hitbox.offset_y),
        static_cast<int>(hitbox.width),
        static_cast<int>(hitbox.height)
    };

    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &PlatformRect);
}