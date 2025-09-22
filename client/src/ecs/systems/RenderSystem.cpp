/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** RenderSystem
*/

#include "RenderSystem.hpp"
#include "../../managers/GraphicsManager.hpp"
#include "ecs/Components.hpp"
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

    auto view = registry.view<Position>();
    for (auto [pos] : view) {
        std::cout << "Rendering entity at position (" << pos.x << ", " << pos.y << ")\n";
        SDL_Rect rect = {
            static_cast<int>(pos.x),
            static_cast<int>(pos.y),
            15, 15
        };

        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderDrawRect(renderer, &rect);
    }
    graphics.present();
}
