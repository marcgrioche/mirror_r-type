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
#include <algorithm>
#include <iostream>
#include <vector>

/**
 * @brief System that manages power-up spawning and collection
 * Handles the creation of power-ups at random intervals and processes player collection
 * @param registry The ECS registry containing all game entities and components
 * @param deltaTime Time elapsed since the last frame in seconds
 */
void powerUpSystem(Registry& registry, float deltaTime);
