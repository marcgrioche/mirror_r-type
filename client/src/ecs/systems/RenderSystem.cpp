/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** RenderSystem
*/

#include "RenderSystem.hpp"
#include "components/Button.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/Tags.hpp"
#include "components/TextBox.hpp"
#include "managers/GraphicsManager.hpp"

void renderSystem(Registry& registry)
{
    auto& graphics = GraphicsManager::getInstance();

    if (!graphics.isInitialized()) {
        return;
    }

    SDL_Renderer* renderer = graphics.getRenderer();

    graphics.clear(20, 30, 50, 255);

    auto view = registry.view<Hitbox, Position>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();
        // Skip UI buttons to avoid white rectangles from appearing in gameplay
        if (registry.has<Button>(e)) {
            continue;
            // choose a different color for each entity to distinguish them
        } else if (registry.has<PlayerTag>(e)) {
            graphics.setDrawColor(255, 0, 255, 255);
        } else if (registry.has<PlatformTag>(e)) {
            graphics.setDrawColor(255, 255, 255, 255);
        } else if (registry.has<ProjectileTag>(e)) {
            graphics.setDrawColor(255, 255, 0, 255);
        } else if (registry.has<EnemyTag>(e)) {
            graphics.setDrawColor(0, 255, 255, 255);
        } else if (registry.has<PowerUpTag>(e)) {
            graphics.setDrawColor(0, 255, 0, 255);
        }

        Hitbox& hitbox = registry.get<Hitbox>(e);
        Position& pos = registry.get<Position>(e);
        SDL_Rect rect = {
            static_cast<int>(pos.x),
            static_cast<int>(pos.y),
            static_cast<int>(hitbox.width), static_cast<int>(hitbox.height)
        };
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderDrawRect(renderer, &rect);
    }
    playerPseudoRenderSystem(registry, renderer);
    graphics.present();
}

void playerPseudoRenderSystem(Registry& registry, SDL_Renderer* renderer)
{
    if (!renderer)
        return;
    auto view = registry.view<TextBox, Position>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();
        TextBox& textBox = registry.get<TextBox>(e);
        Position& pos = registry.get<Position>(e);
        if (!registry.has<PlayerTag>(e)) {
            continue;
        }

        // Prepare SDL_Color
        TTF_Font* font = TTF_OpenFont(textBox.fontPath.c_str(), textBox.fontSize);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            // Essaie un chemin de police par défaut
            font = TTF_OpenFont("client/res/fonts/OpenSans-Medium.ttf", textBox.fontSize);
            if (!font) {
                std::cerr << "Failed to load default font: " << TTF_GetError() << std::endl;
                return;
            }
        }

        SDL_Color color = {
            textBox.color.r,
            textBox.color.g,
            textBox.color.b,
            textBox.color.a
        };

        // Render the text with SDL_ttf
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, textBox.text.c_str(), color);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                SDL_Rect dstRect;
                dstRect.x = static_cast<int>(pos.x);
                dstRect.y = static_cast<int>(pos.y) - textSurface->h; // Render above the entity
                dstRect.w = textSurface->w;
                dstRect.h = textSurface->h;
                SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }
}
