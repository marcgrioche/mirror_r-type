#pragma once
#include <SDL2/SDL_rect.h>

#include "Registry.hpp"

SDL_Rect getTextBoxDimensions(Registry& registry, const Entity& entity);

float getXOffset(const SDL_Rect& rect, float width);
float getYOffset(const SDL_Rect& rect, float height);
