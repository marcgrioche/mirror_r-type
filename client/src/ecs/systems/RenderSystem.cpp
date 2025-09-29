/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** RenderSystem
*/

#include "RenderSystem.hpp"
#include "components/AllComponents.hpp"
#include "managers/GraphicsManager.hpp"
#include <SDL2/SDL.h>

void renderSystem(Registry& registry)
{
    auto& graphics = GraphicsManager::getInstance();

    if (!graphics.isInitialized()) {
        return;
    }

    SDL_Renderer* renderer = graphics.getRenderer();

    graphics.clear(20, 30, 50, 255);

    graphics.setDrawColor(255, 255, 255, 255);
    auto view = registry.view<Hitbox, Position>();
    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();
        // Skip UI buttons to avoid white rectangles from appearing in gameplay
        if (registry.has<Button>(e)) {
            continue;
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
    graphics.present();
}
