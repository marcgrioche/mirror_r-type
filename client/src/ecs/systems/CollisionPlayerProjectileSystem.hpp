/*
** CollisionPlayerProjectileSystem.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 2:19:54 PM 2025 jojo
** Last update Wed Sep 23 2:21:55 PM 2025 jojo
*/

#pragma once
#include "ecs/Components.hpp"
#include "ecs/Hitbox.hpp"
#include "ecs/Registry.hpp"
#include <algorithm>
#include <vector>

/// Detect and handle collisions between projectiles and players.
/// - appel depuis la boucle de jeu après movementSystem(...)
/// - deltaTime fourni pour éventuelles extensions (animation, effets)
void collisionPlayerProjectileSystem(Registry& registry, float deltaTime);
