/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** RenderSystem
*/

#include "RenderSystem.hpp"
#include "GraphicsManager.hpp"
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

    auto& ps = registry.get_or_create_storage<Position>();
    for (size_t i = 0; i < ps.dense_size(); ++i) {
        auto pos = ps.get_by_dense_index(i);

        SDL_Rect rect = {
            static_cast<int>(pos.x * 20 + 100),
            static_cast<int>(pos.y * 20 + 100),
            15, 15
        };

        SDL_RenderFillRect(renderer, &rect);
    }

    graphics.present();
}