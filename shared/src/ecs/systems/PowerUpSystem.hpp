/*
** PowerUpSystem.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 2:06:44 PM 2025 jojo
** Last update Wed Oct 7 2:07:11 PM 2025 jojo
*/

#pragma once

#include "Registry.hpp"
#include "components/AllComponents.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

void powerUpSystem(Registry& registry, float deltaTime);