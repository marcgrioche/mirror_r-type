/*
** PlatformRender.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 6 9:58:54 AM 2025 jojo
** Last update Tue Oct 6 10:11:19 AM 2025 jojo
*/

#pragma once

#include "Registry.hpp"
#include "components/Hitbox.hpp"
#include "components/Platform.hpp"
#include "components/Position.hpp"
#include "components/Tags.hpp"
#include "managers/GraphicsManager.hpp"

/**
 * @brief Draws a platform entity on the screen
 * @param gfx The graphics manager for rendering
 * @param registry The ECS registry containing the entity
 * @param platform The platform entity to draw
 */
void drawPlatform(GraphicsManager& gfx, Registry& registry, Entity platform);
