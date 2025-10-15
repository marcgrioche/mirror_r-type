/*
** CollisionEnemyProjectileSystem.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 2:19:54 PM 2025 jojo
** Last update Wed Sep 23 4:04:11 PM 2025 jojo
*/

#pragma once
#include "components/Hitbox.hpp"
#include "ecs/Registry.hpp"
#include <algorithm>
#include <vector>

/**
 * @brief System that handles collisions between enemies and projectiles
 * Detects when enemy entities are hit by projectiles and applies damage accordingly
 * @param registry The ECS registry containing all game entities and components
 * @param deltaTime Time elapsed since the last frame in seconds
 */
void collisionEnemyProjectileSystem(Registry& registry, float deltaTime);
