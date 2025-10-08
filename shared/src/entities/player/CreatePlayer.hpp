/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Player
*/

#pragma once
#include "Registry.hpp"
#include "components/Position.hpp"
#include "components/Health.hpp"
#include "components/Hitbox.hpp"

namespace factories {
Entity createPlayer(Registry& registry);
Entity createPlayer(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox);
}
