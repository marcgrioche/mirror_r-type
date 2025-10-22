/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** WeaponRotationSystem - Updates weapon sprite rotation to aim at mouse cursor
*/

#pragma once
#include "Registry.hpp"

namespace WeaponRotationSystem {

void updateWeaponRotations(Registry& registry, float mouseX, float mouseY);

void applyNetworkWeaponRotations(Registry& registry);

}
