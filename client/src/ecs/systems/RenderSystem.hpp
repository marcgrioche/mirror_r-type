/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** RenderSystem
*/

#pragma once
#include "Registry.hpp"
#include <SDL.h>

void renderSystem(Registry& registry);
void playerPseudoRenderSystem(Registry& registry, SDL_Renderer* renderer);
