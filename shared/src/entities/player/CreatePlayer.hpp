/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Player
*/

#pragma once
#include "Registry.hpp"
#include "components/AllComponents.hpp"

namespace factories {
Entity createPlayer(std::shared_ptr<Registry> registry);
Entity createPlayer(std::shared_ptr<Registry> registry, const Position& position, const Health& health, const Hitbox& hitbox);
}
