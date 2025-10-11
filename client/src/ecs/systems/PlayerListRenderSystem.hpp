#pragma once

#include "Registry.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Render all players' names and HP bars at the top left of the window
void playerListRenderSystem(Registry& registry, SDL_Renderer* renderer);
