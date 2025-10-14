/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** WeaponPositionSystem
*/

#include "WeaponPositionSystem.hpp"
#include "Tags.hpp"
#include "Parent.hpp"
#include "Position.hpp"

void weaponPositionSystem(Registry& registry)
{
    auto view = registry.view<WeaponTag, Parent, Position>();

    for (auto [w, parent, position] : view) {
        if (registry.has<Position>(parent.parent)) {
            auto& parentPosition = registry.get<Position>(parent.parent);
            position.v.x = parentPosition.v.x;
            position.v.y = parentPosition.v.y;
        }
    }
}
