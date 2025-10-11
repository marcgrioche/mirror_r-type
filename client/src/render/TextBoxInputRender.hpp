/*
** TextBoxInputRender.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 4:58:40 PM 2025 jojo
** Last update Wed Oct 7 5:01:19 PM 2025 jojo
*/

#pragma once

#include "Registry.hpp"
#include "TextBoxRender.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/Tags.hpp"
#include "components/TextBox.hpp"
#include "components/TextBoxInput.hpp"
#include "managers/GraphicsManager.hpp"

/**
 * @brief Draws a text box input entity on the screen
 * @param gfx The graphics manager for rendering
 * @param registry The ECS registry containing the entity
 * @param entity The entity to draw
 */
void drawTextBoxInput(GraphicsManager& gfx, Registry& registry, Entity entity);
