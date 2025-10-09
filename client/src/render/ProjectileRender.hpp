/*
** ProjectileRender.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/render
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Oct 6 10:00:20 AM 2025 jojo
** Last update Tue Oct 6 10:00:28 AM 2025 jojo
*/

#pragma once

#include "Registry.hpp"
#include "components/Hitbox.hpp"
#include "components/Platform.hpp"
#include "components/Position.hpp"
#include "managers/GraphicsManager.hpp"

void drawProjectil(GraphicsManager& gfx, Registry& registry, Entity platform);