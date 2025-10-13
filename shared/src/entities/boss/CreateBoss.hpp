/*
** CreateBoss.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/entities/boss
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 8 2:53:37 PM 2025 jojo
** Last update Thu Oct 8 3:21:29 PM 2025 jojo
*/

#pragma once
#include "../../ecs/systems/FrequencyUtils.hpp"
#include "../components/Frequency.hpp"
#include "../enemies/CreateEnemy.hpp"
#include "../weapon/CreateWeapon.hpp"
#include "Config.hpp"
#include "Registry.hpp"
#include "components/Dead.hpp"
#include "components/Health.hpp"
#include "components/Hitbox.hpp"
#include "components/Lifetime.hpp"
#include "components/Position.hpp"
#include "components/PreviousPosition.hpp"
#include "components/Tags.hpp"
#include "components/Velocity.hpp"
#include <iostream>

namespace factories {
/**
 * @brief Creates a boss entity with default properties at the right edge of the screen
 * @param registry The ECS registry to create the entity in
 * @return The created boss entity
 */
Entity createBoss(Registry& registry);

/**
 * @brief Creates a boss entity with custom properties
 * @param registry The ECS registry to create the entity in
 * @param position The initial position of the boss
 * @param health The health component for the boss
 * @param hitbox The hitbox component for collision detection
 * @param velocity The velocity component for movement
 * @return The created boss entity
 */
Entity createBoss(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox, const Velocity& velocity);
}
