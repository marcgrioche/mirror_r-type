/*
** TextBoxRender.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 6 2:46:51 PM 2025 jojo
** Last update Tue Oct 6 3:03:03 PM 2025 jojo
*/

#include "TextBoxRender.hpp"

void drawTextBox(GraphicsManager& gfx, Registry& registry, Entity entity)
{

    SDL_Renderer* renderer = gfx.getRenderer();

    if (!registry.has<TextBoxTag>(entity))
        return;

    const TextBox& textbox = registry.get<TextBox>(entity);
    const Position& pos = registry.get<Position>(entity);
    const Hitbox& hitbox = registry.get<Hitbox>(entity);

    if (!textbox.visible)
        return;

    TTF_Font* font = TTF_OpenFont(textbox.fontPath.c_str(), textbox.fontSize);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color color = { textbox.color.a, textbox.color.r, textbox.color.g, textbox.color.b };

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textbox.text.c_str(), color);
    if (!textSurface) {
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    int renderX = static_cast<int>(pos.x);

    switch (textbox.alignment) {
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

    SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}