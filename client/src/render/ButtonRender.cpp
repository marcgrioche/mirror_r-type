/*
** ButtonRender.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 5:20:40 PM 2025 jojo
** Last update Tue Oct 6 10:12:58 AM 2025 jojo
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

    SDL_Color bgColor, borderColor;

    switch (button.state) {
    case ButtonState::Idle:
        bgColor = { 100, 100, 100, 255 }; // gris
        borderColor = { 150, 150, 150, 255 }; // gris clair
        break;
    case ButtonState::Hovered:
        bgColor = { 130, 130, 130, 255 }; // gris plus clair
        borderColor = { 200, 200, 200, 255 }; // gris très clair
        break;
    case ButtonState::Pressed:
        bgColor = { 70, 70, 70, 255 }; // gris foncé
        borderColor = { 100, 100, 100, 255 }; // gris moyen
        break;
    case ButtonState::Disabled:
        bgColor = { 50, 50, 50, 255 }; // gris très foncé
        borderColor = { 80, 80, 80, 255 }; // gris sombre
        break;
    }

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

    // Effet d'enfoncement pour l'état pressé
    if (button.state == ButtonState::Pressed) {
        SDL_Rect innerRect = {
            buttonRect.x + 1,
            buttonRect.y + 1,
            buttonRect.w - 2,
            buttonRect.h - 2
        };
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderDrawRect(renderer, &innerRect);
    }
}
