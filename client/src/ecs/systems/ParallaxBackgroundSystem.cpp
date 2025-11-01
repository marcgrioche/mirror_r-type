/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** ParallaxBackgroundSystem - Handles parallax background rendering
*/

#include "ParallaxBackgroundSystem.hpp"
#include "../../../shared/include/Config.hpp" // For PLATFORM_VELOCITY_X
#include "Config.hpp"
#include "components/ParallaxState.hpp"
#include "managers/GraphicsManager.hpp"
#include "managers/ResourceManager.hpp"
#include <SDL.h>
#include <iostream>

void parallaxBackgroundSystem(Registry& registry)
{
    auto& graphics = GraphicsManager::getInstance();
    auto& resourceManager = ResourceManager::getInstance();
    SDL_Renderer* renderer = graphics.getRenderer();

    auto parallaxView = registry.view<ParallaxState>();
    Entity parallaxEntity = parallaxView.begin().entity();
    ParallaxState& parallaxState = registry.get<ParallaxState>(parallaxEntity);

    struct LayerInfo {
        std::string textureId;
        float scrollSpeed;
        float* offsetPtr;
    };

    // Resolve IDs from state with safe fallbacks to default IDs
    const std::string skyId = parallaxState.skyId.empty() ? std::string("Sky.png") : parallaxState.skyId;
    const std::string downId = parallaxState.downLayerId.empty() ? std::string("DownLayer.png") : parallaxState.downLayerId;
    const std::string midId = parallaxState.middleLayerId.empty() ? std::string("MiddleLayer.png") : parallaxState.middleLayerId;
    const std::string lightId = parallaxState.lightId.empty() ? std::string("Light.png") : parallaxState.lightId;
    const std::string topId = parallaxState.topLayerId.empty() ? std::string("TopLayer.png") : parallaxState.topLayerId;

    std::vector<LayerInfo> layers = {
        { skyId, 0.2f, &parallaxState.skyOffset }, // Sky: 20% of platform speed
        { downId, 0.4f, &parallaxState.downLayerOffset }, // Down layer: 40% of platform speed
        { midId, 0.6f, &parallaxState.middleLayerOffset }, // Middle layer: 60% of platform speed
        { lightId, 0.8f, &parallaxState.lightOffset }, // Light: 80% of platform speed
        { topId, 1.0f, &parallaxState.topLayerOffset } // Top layer: 100% of platform speed (same as platforms)
    };

    // Use platform velocity from config (positive speed)
    float platformSpeed = -PLATFORM_VELOCITY_X; // Convert negative velocity to positive speed

    // Update layer offsets based on platform movement (assuming 60 FPS)
    float deltaTime = 1.0f / 60.0f;
    for (auto& layer : layers) {
        *layer.offsetPtr -= platformSpeed * layer.scrollSpeed * deltaTime;
    }

    // Render each layer
    for (const auto& layer : layers) {
        SDL_Texture* texture = resourceManager.getTexture(layer.textureId);
        if (!texture) {
            continue; // Skip if texture not loaded
        }

        // Get texture dimensions
        int textureWidth, textureHeight;
        SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

        // Calculate how many tiles we need to cover the screen
        int screenWidth = SCREEN_WIDTH; // Should get from GraphicsManager
        int tilesNeeded = (screenWidth / textureWidth) + 2; // +2 for seamless scrolling

        // Render tiled background
        for (int i = -1; i < tilesNeeded; ++i) { // Start from -1 to handle negative offsets
            int x = static_cast<int>(i * textureWidth + *layer.offsetPtr);

            // Skip tiles that are completely off-screen
            if (x + textureWidth < 0 || x > screenWidth) {
                continue;
            }

            SDL_Rect dstRect = {
                x,
                0, // Background starts at top
                textureWidth,
                SCREEN_HEIGHT // Screen height
            };

            SDL_Rect srcRect = {
                0, 0,
                textureWidth,
                textureHeight
            };

            SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
        }
    }

    // Reset offsets when they get too large (to prevent floating point precision issues)
    for (auto& layer : layers) {
        int textureWidth = 0;
        SDL_Texture* texture = resourceManager.getTexture(layer.textureId);
        if (texture) {
            SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, nullptr);
        }

        if (std::abs(*layer.offsetPtr) > textureWidth) {
            *layer.offsetPtr = std::fmod(*layer.offsetPtr, static_cast<float>(textureWidth));
        }
    }
}
