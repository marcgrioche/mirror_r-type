/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** RenderSystem
*/

#include "RenderSystem.hpp"
#include "managers/GraphicsManager.hpp"
#include "managers/ResourceManager.hpp"
#include "components/AllComponents.hpp"
#include <SDL2/SDL.h>

void renderSystem(Registry& registry)
{
    auto& graphics = GraphicsManager::getInstance();
    auto& resourceManager = ResourceManager::getInstance();

    if (!graphics.isInitialized()) return;

    SDL_Renderer* renderer = graphics.getRenderer();
    graphics.clear(20, 30, 50, 255);

    auto view = registry.view<Position, Sprite>();
    for (auto [pos, sprite] : view) {
        SDL_Texture* tex = resourceManager.getTexture(sprite.texture_id);
        sprite.dstRect.x = pos.x;
        sprite.dstRect.y = pos.y;
        if (tex) {
            SDL_RenderCopy(renderer, tex, &sprite.srcRect, &sprite.dstRect);
        }
    }

    graphics.present();
}
