/*
** TextBoxInputRender.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 4:59:15 PM 2025 jojo
** Last update Tue Oct 20 4:52:27 PM 2025 jojo
*/

#include "TextBoxInputRender.hpp"
#include "SpriteRender.hpp"
#include "components/Sprite.hpp"

void drawTextBoxInput(GraphicsManager& gfx, Registry& registry, Entity entity)
{
    SDL_Renderer* renderer = gfx.getRenderer();

    // Vérifie que l'entité a les components nécessaires
    if (!registry.has<TextBoxInput>(entity) || !registry.has<TextBox>(entity) || !registry.has<Position>(entity) || !registry.has<Hitbox>(entity)) {
        return;
    }

    const TextBoxInput& input = registry.get<TextBoxInput>(entity);
    const TextBox& textBox = registry.get<TextBox>(entity);
    const Position& pos = registry.get<Position>(entity);
    const Hitbox& hitbox = registry.get<Hitbox>(entity);

    SDL_Rect inputRect = {
        static_cast<int>(pos.v.x + hitbox.offset_x),
        static_cast<int>(pos.v.y + hitbox.offset_y),
        static_cast<int>(hitbox.width),
        static_cast<int>(hitbox.height)
    };

    // 1. Dessine le fond et la bordure de la TextBox
    drawTextBox(gfx, registry, entity); // Passe registry et entity, pas textBox et pos

    if (input.isFocused && input.showCursor) {

        int charWidth = textBox.fontSize * 0.6f; // estimation largeur caractère
        int cursorX = inputRect.x + 5 + static_cast<int>(input.cursorPosition * charWidth);

        // Ligne du curseur
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // noir
        SDL_RenderDrawLine(renderer,
            cursorX, inputRect.y + 3,
            cursorX, inputRect.y + inputRect.h - 3);
    }
}
