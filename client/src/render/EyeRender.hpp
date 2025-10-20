/*
** EyeRender.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 20 8:52:49 PM 2025 jojo
** Last update Tue Oct 20 8:56:51 PM 2025 jojo
*/

#include "Registry.hpp"
#include "SpriteRender.hpp"
#include "components/Eye.hpp"
#include "components/Position.hpp"
#include "components/Sprite.hpp"
#include "components/Tags.hpp"
#include "managers/GraphicsManager.hpp"

#pragma once

void drawEye(GraphicsManager& gfx, Registry& registry, Entity sprite);
