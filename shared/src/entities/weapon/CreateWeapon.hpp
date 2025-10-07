/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** CreateWeapon
*/

#pragma once
#include "Entity.hpp"
#include "Parent.hpp"
#include "Registry.hpp"
#include <optional>

// always a parent so they can be turrets
namespace factories {
Entity createWeapon(std::shared_ptr<Registry> registry);
Entity createWeapon(std::shared_ptr<Registry> registry, Parent parent);
}
