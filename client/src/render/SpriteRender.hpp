/*
** SpriteRender.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 15 5:45:20 PM 2025 jojo
** Last update Thu Oct 15 5:49:41 PM 2025 jojo
*/

#pragma once

#include "Registry.hpp"
#include "components/Position.hpp"
#include "components/Sprite.hpp"
#include "components/Tags.hpp"
#include "managers/GraphicsManager.hpp"

/**
 * @brief Draws a Sprite entity on the screen
 * @param gfx The graphics manager for rendering
 * @param registry The ECS registry containing the entity
 * @param sprite The Sprite entity to draw
 */
void drawSprite(GraphicsManager& gfx, Registry& registry, Entity sprite);
