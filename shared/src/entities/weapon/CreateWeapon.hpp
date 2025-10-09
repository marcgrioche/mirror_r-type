/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** CreateWeapon
*/

#pragma once
#include "Entity.hpp"
#include "Registry.hpp"
#include <optional>
#include "Parent.hpp"

//always a parent so they can be turrets
namespace factories {
    Entity createWeapon(Registry &registry);
    Entity createWeapon(Registry &registry, Parent parent, Entity projectile);
}
