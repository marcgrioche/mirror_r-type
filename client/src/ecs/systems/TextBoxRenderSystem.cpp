/*
** RenderTextBoxSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 1:24:26 PM 2025 jojo
** Last update Tue Oct 6 10:48:43 AM 2025 jojo
*/

#include "TextBoxRenderSystem.hpp"
#include <SDL2/SDL_ttf.h>

void textRenderSystem(Registry& registry, GraphicsManager& graphics)
{
    auto view = registry.view<TextBox, Position>();

    for (auto [textBox, pos] : view) {
        if (!textBox.visible)
            continue;

        TTF_Font* font = TTF_OpenFont(textBox.fontPath.c_str(), textBox.fontSize);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            continue;
        }

        SDL_Color color = { textBox.color.a, textBox.color.r, textBox.color.g, textBox.color.b };

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, textBox.text.c_str(), color);
        if (!textSurface) {
            TTF_CloseFont(font);
            continue;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), textSurface);

        int textWidth = textSurface->w;
        int textHeight = textSurface->h;
        int renderX = static_cast<int>(pos.x);

        switch (textBox.alignment) {
        case TextBox::Alignment::CENTER:
            renderX -= textWidth / 2;
            break;
        case TextBox::Alignment::RIGHT:
            renderX -= textWidth;
            break;
        case TextBox::Alignment::LEFT:
        default:
            break;
        }

        SDL_Rect dstRect = { renderX, static_cast<int>(pos.y), textWidth, textHeight };

        // Rendu
        SDL_RenderCopy(graphics.getRenderer(), textTexture, nullptr, &dstRect);

        // Cleanup
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
    }
}
// ...existing code...