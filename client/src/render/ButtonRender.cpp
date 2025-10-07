/*
** ButtonRender.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 5:20:40 PM 2025 jojo
** Last update Tue Oct 6 3:22:22 PM 2025 jojo
*/

#include "ButtonRender.hpp"

void drawButton(GraphicsManager& gfx, Registry& registry, Entity entity)
{
    SDL_Renderer* renderer = gfx.getRenderer();

    if (!registry.has<ButtonTag>(entity))
        return;

    const Button& button = registry.get<Button>(entity);
    const Position& pos = registry.get<Position>(entity);
    const Hitbox& hitbox = registry.get<Hitbox>(entity);

    SDL_Color bgColor;
    SDL_Color borderColor;

    // Rectangle du bouton
    SDL_Rect buttonRect = {
        static_cast<int>(pos.x + hitbox.offset_x),
        static_cast<int>(pos.y + hitbox.offset_y),
        static_cast<int>(hitbox.width),
        static_cast<int>(hitbox.height)
    };

    // Dessine le fond du bouton
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &buttonRect);

    // Dessine la bordure
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(renderer, &buttonRect);
}
