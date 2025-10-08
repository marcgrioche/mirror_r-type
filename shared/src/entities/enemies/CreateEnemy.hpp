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
#include "Registry.hpp"
#include "Config.hpp"
#include "components/Position.hpp"
#include "../weapon/CreateWeapon.hpp"
#include "components/Position.hpp"
#include "components/PreviousPosition.hpp"
#include "components/Velocity.hpp"
#include "components/Health.hpp"
#include "components/Hitbox.hpp"
#include "components/Lifetime.hpp"
#include "components/Dead.hpp"
#include "components/Tags.hpp"
#include "../components/Frequency.hpp"
#include "../../ecs/systems/FrequencyUtils.hpp"
#include <iostream>

namespace factories {
Entity createEnemy(Registry& registry);
Entity createEnemy(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox, const Velocity& velocity);
}