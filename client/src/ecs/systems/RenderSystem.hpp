/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** RenderSystem
*/

#pragma once
#include "Registry.hpp"
#include <SDL.h>

/**
 * @brief Main rendering system that processes all renderable entities
 * @param registry The ECS registry containing entities to render
 */
void renderSystem(Registry& registry);

/**
 * @brief Renders player pseudonyms/names on screen
 * @param registry The ECS registry containing player entities
 * @param renderer The SDL renderer to use for drawing
 */
void playerPseudoRenderSystem(Registry& registry, SDL_Renderer* renderer);
