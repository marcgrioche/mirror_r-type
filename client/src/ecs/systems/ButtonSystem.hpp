/*
** ButtonSystem.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Sep 29 1:02:51 PM 2025 jojo
** Last update Tue Sep 29 1:05:10 PM 2025 jojo
*/

#pragma once
#include "../../managers/EventManager.hpp"
#include "components/Button.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "ecs/Registry.hpp"
#include <SDL.h>

void buttonSystem(Registry& registry);
