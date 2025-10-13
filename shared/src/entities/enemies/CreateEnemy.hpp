/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Oct, 2025                                                     *
 * Title           - mirror_r-type                                                      *
 * Description     -                                                                    *
 *     Enemy                                                                    *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#pragma once
#include "../../ecs/systems/FrequencyUtils.hpp"
#include "../components/Frequency.hpp"
#include "../projectile/CreateProjectile.hpp"
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
 * @brief Creates an enemy entity with default properties at the right edge of the screen
 * @param registry The ECS registry to create the entity in
 * @return The created enemy entity
 */
Entity createEnemy(Registry& registry);

/**
 * @brief Creates an enemy entity with custom properties
 * @param registry The ECS registry to create the entity in
 * @param position The initial position of the enemy
 * @param health The health component for the enemy
 * @param hitbox The hitbox component for collision detection
 * @param velocity The velocity component for movement
 * @return The created enemy entity
 */
Entity createEnemy(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox, const Velocity& velocity);

/**
 * @brief Creates an enemy template entity without position or weapon (used as blueprint)
 * @param registry The ECS registry to create the entity in
 * @param health The health component for the enemy
 * @param hitbox The hitbox component for collision detection
 * @param velocity The velocity component for movement
 * @return The created enemy template entity
 */
Entity createEnemyTemplate(Registry& registry, const Health& health, const Hitbox& hitbox, const Velocity& velocity);
}
