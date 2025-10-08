/*
** CollisionPlayerProjectileSystem.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 2:19:54 PM 2025 jojo
** Last update Wed Sep 23 4:04:11 PM 2025 jojo
*/

#pragma once
#include "components/Components.hpp"
#include "components/Hitbox.hpp"
#include "ecs/Registry.hpp"
#include <algorithm>
#include <vector>

void collisionEnemyProjectileSystem(Registry& registry, float deltaTime);
