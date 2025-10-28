/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** WeaponRotationSystem - Rotates weapon sprites toward mouse cursor
*/

#pragma once

#include "Registry.hpp"

namespace WeaponRotationSystem {
    /**
     * @brief Updates weapon rotation to point toward mouse cursor
     * @param registry The ECS registry containing entities
     * @param mouseX Mouse X position in screen coordinates
     * @param mouseY Mouse Y position in screen coordinates
     */
    void updateWeaponRotations(Registry& registry, float mouseX, float mouseY);
}
