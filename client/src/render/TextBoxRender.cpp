/*
** TextBoxRender.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 6 2:46:51 PM 2025 jojo
** Last update Wed Oct 7 8:35:43 PM 2025 jojo
*/

#include "TextBoxRender.hpp"

void drawTextBox(GraphicsManager& gfx, Registry& registry, Entity entity)
{
    SDL_Renderer* renderer = gfx.getRenderer();

    // SUPPRIME cette vérification - TextBoxTag n'existe pas
    // if (!registry.has<TextBoxTag>(entity))
    //     return;

    // Vérifie seulement les components nécessaires
    if (!registry.has<TextBox>(entity) || !registry.has<Position>(entity)) {
        return;
    }

    const TextBox& textbox = registry.get<TextBox>(entity);
    const Position& pos = registry.get<Position>(entity);

    // Utilise Hitbox seulement si présent. USELESS 'CAUSE UNUSED !!!!
    // float width = 200.0f; // largeur par défaut
    // float height = 30.0f; // hauteur par défaut
    // if (registry.has<Hitbox>(entity)) {
    //     const Hitbox& hitbox = registry.get<Hitbox>(entity);
    //     width = hitbox.width;
    //     height = hitbox.height;
    // }

    if (!textbox.visible || textbox.text.empty())
        return;

    TTF_Font* font = TTF_OpenFont(textbox.fontPath.c_str(), textbox.fontSize);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        // Essaie un chemin de police par défaut
        font = TTF_OpenFont("res/fonts/OpenSans-Medium.ttf", textbox.fontSize);
        if (!font) {
            std::cerr << "Failed to load default font: " << TTF_GetError() << std::endl;
            return;
        }
    }

    // CORRIGE l'ordre des couleurs : SDL utilise (r, g, b, a) pas (a, r, g, b)
    SDL_Color color = {
        textbox.color.r,
        textbox.color.g,
        textbox.color.b,
        textbox.color.a
    };

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textbox.text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    // Position et taille de rendu
    SDL_Rect destRect = {
        static_cast<int>(pos.v.x),
        static_cast<int>(pos.v.y),
        textSurface->w,
        textSurface->h
    };

    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

    // Nettoyage
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}