/*
** CreateProjectile.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/projectil
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 1:12:31 PM 2025 jojo
** Last update Wed Sep 23 1:12:32 PM 2025 jojo
*/

#pragma once
#include "ecs/components/Components.hpp"
#include "ecs/Registry.hpp"

namespace factories {
void createProjectile(Registry& registry, Vector2 position, Vector2 velocity,
    Entity owner, float lifetime = 3.0f, int damage = 1);
}
