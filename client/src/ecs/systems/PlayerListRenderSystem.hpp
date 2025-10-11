#pragma once

#include "Registry.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * @brief System that renders player list with names and health bars
 * Displays all connected players' names and health status in the top-left corner of the screen
 * @param registry The ECS registry containing all game entities and components
 * @param renderer The SDL renderer used for drawing the player list
 */
void playerListRenderSystem(Registry& registry, SDL_Renderer* renderer);
