/*
** ButtonRender.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 5:18:09 PM 2025 jojo
** Last update Tue Oct 6 10:12:52 AM 2025 jojo
*/

#pragma once

#include "Registry.hpp"
#include "components/Button.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/Tags.hpp"
#include "managers/GraphicsManager.hpp"

/**
 * @brief Draws a button entity on the screen
 * @param gfx The graphics manager for rendering
 * @param registry The ECS registry containing the entity
 * @param button The button entity to draw
 */
void drawButton(GraphicsManager& gfx, Registry& registry, Entity button);
