/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Projectile Type - Links projectiles to their originating weapon/entity
*/

#include "Entity.hpp"

#pragma once

/**
 * @brief Component linking projectiles to their originating weapon or entity
 * @param entity The entity that fired/created this projectile (weapon or shooter)
 */
struct ProjectileType {
    Entity entity;
};
