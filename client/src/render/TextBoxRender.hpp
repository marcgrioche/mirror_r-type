/*
** TextBoxRender.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 6 2:43:31 PM 2025 jojo
** Last update Tue Oct 6 2:52:21 PM 2025 jojo
*/

#pragma once

#include "Registry.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/Tags.hpp"
#include "components/TextBox.hpp"
#include "managers/GraphicsManager.hpp"

void drawTextBox(GraphicsManager& gfx, Registry& registry, Entity entity);