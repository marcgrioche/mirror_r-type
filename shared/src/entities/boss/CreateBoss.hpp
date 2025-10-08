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
Entity createBoss(Registry& registry);
Entity createBoss(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox);
}
