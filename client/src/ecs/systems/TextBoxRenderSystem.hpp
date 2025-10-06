/*
** RenderTextBoxSystem.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 11:24:31 AM 2025 jojo
** Last update Thu Oct 1 1:24:15 PM 2025 jojo
*/

#pragma once

#include "components/AllComponents.hpp"
#include "managers/GraphicsManager.hpp"
#include <SDL2/SDL_ttf.h>

void textRenderSystem(Registry& registry, GraphicsManager& graphics);
