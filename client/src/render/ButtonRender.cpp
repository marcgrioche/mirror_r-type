/*
** ButtonRender.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 5:20:40 PM 2025 jojo
** Last update Tue Oct 20 1:09:57 PM 2025 jojo
*/

#include "ButtonRender.hpp"
#include "SpriteRender.hpp"
#include "components/Sprite.hpp"

void drawButton(GraphicsManager& gfx, Registry& registry, Entity entity)
{
    SDL_Renderer* renderer = gfx.getRenderer();

    if (!registry.has<Button>(entity) || !registry.has<Position>(entity) || !registry.has<Hitbox>(entity))
        return;

    const Button& button = registry.get<Button>(entity);
    const Position& pos = registry.get<Position>(entity);
    const Hitbox& hitbox = registry.get<Hitbox>(entity);

    if (registry.has<Sprite>(entity)) {
        drawSprite(gfx, registry, entity);
    } else {
        SDL_Color bgColor;
        SDL_Color borderColor;

        // Couleurs selon l'état
        if (button.is_hovered) {
            bgColor = { 200, 220, 255, 255 }; // Bleu clair
            borderColor = { 100, 150, 255, 255 }; // Bleu foncé
        } else {
            bgColor = { 120, 120, 120, 255 }; // Gris
            borderColor = { 80, 80, 80, 255 }; // Gris foncé
        }

        SDL_Rect buttonRect = {
            static_cast<int>(pos.v.x + hitbox.offset_x),
            static_cast<int>(pos.v.y + hitbox.offset_y),
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
}
