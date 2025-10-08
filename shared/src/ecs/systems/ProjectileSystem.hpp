/*
** ProjectileSystem.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 1:12:19 PM 2025 jojo
** Last update Wed Sep 23 1:14:24 PM 2025 jojo
*/

#pragma once
#include "Registry.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

// Projectile system - updates entity projectile
void projectileSystem(Registry& registry, float deltaTime);