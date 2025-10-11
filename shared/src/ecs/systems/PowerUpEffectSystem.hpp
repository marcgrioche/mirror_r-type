/*
** PowerUpEffectSystem.hpp for mirror_r-type
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 7 5:14:00 PM 2025 jojo
** Last update Wed Oct 7 5:14:00 PM 2025 jojo
*/

#pragma once

#include "Registry.hpp"
#include "components/Parent.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

/**
 * @brief System that manages active power-up effects on entities
 * Updates and applies ongoing effects from collected power-ups to affected entities
 * @param registry The ECS registry containing all game entities and components
 * @param deltaTime Time elapsed since the last frame in seconds
 */
void powerUpEffectSystem(Registry& registry, float deltaTime);
