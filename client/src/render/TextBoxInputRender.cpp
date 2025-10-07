/*
** TextBoxInputRender.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 4:59:15 PM 2025 jojo
** Last update Wed Oct 7 5:04:30 PM 2025 jojo
*/

#include "TextBoxInputRender.hpp"

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

    // 1. Dessine le fond et la bordure de la TextBox
    SDL_Rect inputRect = {
        static_cast<int>(pos.x + hitbox.offset_x),
        static_cast<int>(pos.y + hitbox.offset_y),
        static_cast<int>(hitbox.width),
        static_cast<int>(hitbox.height)
    };

    // Couleur de fond selon l'état
    SDL_Color bgColor = input.isFocused ? SDL_Color { 240, 240, 240, 255 } : // blanc cassé si focus
        SDL_Color { 220, 220, 220, 255 }; // gris clair sinon

    // Couleur de bordure selon l'état (utilise les champs Color du component)
    SDL_Color borderColor = input.isFocused ? SDL_Color { input.focusColor.r, input.focusColor.g, input.focusColor.b, input.focusColor.a } : SDL_Color { input.normalColor.r, input.normalColor.g, input.normalColor.b, input.normalColor.a };

    // Dessine le fond
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &inputRect);

    // Dessine la bordure (plus épaisse si focus)
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(renderer, &inputRect);

    if (input.isFocused) {
        // Bordure double pour le focus
        SDL_Rect innerBorder = {
            inputRect.x + 1, inputRect.y + 1,
            inputRect.w - 2, inputRect.h - 2
        };
        SDL_RenderDrawRect(renderer, &innerBorder);
    }

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
