/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** RenderSystem
*/

#include "RenderSystem.hpp"
#include "ParallaxBackgroundSystem.hpp"
#include "components/Button.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/Sprite.hpp"
#include "components/Tags.hpp"
#include "components/TextBox.hpp"
#include "managers/GraphicsManager.hpp"
#include "managers/ResourceManager.hpp"
#include <SDL.h>

void renderSystem(Registry& registry)
{
    auto& graphics = GraphicsManager::getInstance();

    if (!graphics.isInitialized()) {
        return;
    }

    SDL_Renderer* renderer = graphics.getRenderer();
    auto& resourceManager = ResourceManager::getInstance();

    graphics.clear(20, 30, 50, 255);

    parallaxBackgroundSystem(registry);

    auto spriteView = registry.view<Sprite, Position>();
    for (auto it = spriteView.begin(); it != spriteView.end(); ++it) {
        Entity e = it.entity();
        Sprite& sprite = registry.get<Sprite>(e);
        Position& pos = registry.get<Position>(e);

        SDL_Texture* texture = resourceManager.getTexture(sprite.texture_id);

        SDL_Rect dstRect = {
            static_cast<int>(pos.x + sprite.offset_x),
            static_cast<int>(pos.y + sprite.offset_y),
            static_cast<int>(sprite.dstRect.w * sprite.scale_x),
            static_cast<int>(sprite.dstRect.h * sprite.scale_y)
        };

        if (dstRect.w == 0) {
            dstRect.w = static_cast<int>(sprite.srcRect.w * sprite.scale_x);
        }
        if (dstRect.h == 0) {
            dstRect.h = static_cast<int>(sprite.srcRect.h * sprite.scale_y);
        }

        if (texture) {
            SDL_RenderCopyEx(renderer, texture, &sprite.srcRect, &dstRect,
                sprite.rotation, nullptr, SDL_FLIP_NONE);
        } else {
            if (registry.has<PlayerTag>(e)) {
                graphics.setDrawColor(255, 0, 255, 255); // Magenta for players
            } else if (registry.has<PlatformTag>(e)) {
                graphics.setDrawColor(255, 255, 255, 255); // White for platforms
            } else if (registry.has<ProjectileTag>(e)) {
                graphics.setDrawColor(255, 255, 0, 255); // Yellow for projectiles
            } else if (registry.has<EnemyTag>(e)) {
                graphics.setDrawColor(0, 255, 255, 255); // Cyan for enemies
            } else if (registry.has<PowerUpTag>(e)) {
                graphics.setDrawColor(0, 255, 0, 255); // Green for power-ups
            } else {
                graphics.setDrawColor(128, 128, 128, 255); // Gray for others
            }
            SDL_RenderFillRect(renderer, &dstRect);
            SDL_RenderDrawRect(renderer, &dstRect);
        }
    }

    auto debugView = registry.view<Hitbox, Position>();
    for (auto it = debugView.begin(); it != debugView.end(); ++it) {
        Entity e = it.entity();

        if (registry.has<Button>(e) || registry.has<Sprite>(e)) {
            continue;
        }

        if (registry.has<PlayerTag>(e)) {
            graphics.setDrawColor(255, 0, 255, 255);
        } else if (registry.has<PlatformTag>(e)) {
            graphics.setDrawColor(255, 255, 255, 255);
        } else if (registry.has<ProjectileTag>(e)) {
            graphics.setDrawColor(255, 255, 0, 255);
        } else if (registry.has<EnemyTag>(e)) {
            graphics.setDrawColor(0, 255, 255, 255);
        } else if (registry.has<PowerUpTag>(e)) {
            graphics.setDrawColor(0, 255, 0, 255);
        } else {
            graphics.setDrawColor(128, 128, 128, 255);
        }

        Hitbox& hitbox = registry.get<Hitbox>(e);
        Position& pos = registry.get<Position>(e);
        SDL_Rect rect = {
            static_cast<int>(pos.x + hitbox.offset_x),
            static_cast<int>(pos.y + hitbox.offset_y),
            static_cast<int>(hitbox.width),
            static_cast<int>(hitbox.height)
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
    auto& resourceManager = ResourceManager::getInstance();
    auto view = registry.view<TextBox, Position>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();
        TextBox& textBox = registry.get<TextBox>(e);
        Position& pos = registry.get<Position>(e);
        if (!registry.has<PlayerTag>(e)) {
            continue;
        }

        // Prepare SDL_Color
        TTF_Font* font = resourceManager.getFont(textBox.fontSize);

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
                dstRect.y = static_cast<int>(pos.y) - textSurface->h - 4.f; // Render above the entity
                dstRect.w = textSurface->w;
                dstRect.h = textSurface->h;
                SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }
}
