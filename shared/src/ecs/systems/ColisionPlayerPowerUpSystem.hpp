/*
** ColisionPlayerPowerUpSystem.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 1:55:52 PM 2025 jojo
** Last update Wed Oct 7 1:56:24 PM 2025 jojo
*/

#pragma once
#include "components/Components.hpp"
#include "components/Hitbox.hpp"
#include "ecs/Registry.hpp"
#include <algorithm>
#include <vector>

/**
 * @brief System that handles collisions between players and power-ups
 * Detects when players come into contact with power-up entities and applies power-up effects
 * @param registry The ECS registry containing all game entities and components
 * @param deltaTime Time elapsed since the last frame in seconds
 */
void collisionPlayerPowerUpSystem(Registry& registry, float deltaTime);
