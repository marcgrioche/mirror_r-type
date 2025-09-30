/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** HandleWeaponInputs
*/

#include "HandleWeaponInputs.hpp"
#include "Tags.hpp"
#include "ecs/components/Magazine.hpp"
#include "Damage.hpp"
#include "ecs/components/Frequency.hpp"
#include "entities/projectile/CreateProjectile.hpp"
#include "Velocity.hpp"
#include "Position.hpp"
#include "Parent.hpp"
#include "systems/FrequencyUtils.hpp"

void handleWeaponInputs(InputManager& _inputs, Registry& registry)
{
    auto view = registry.view<WeaponTag, Magazine, Damage, Frequency, Parent>();

    for (auto it = view.begin(); it != view.end();) {
        auto [w, magazine, damage, frequency, parent] = *it;
        if (_inputs.isShoot()) {
            if (FrequencyUtils::triggerAndReset(frequency)) {
                auto entity = it.entity();
                if (registry.has<Position>(parent.parent)) {
                    const Position& parentPosition = registry.get<Position>(parent.parent);
                    factories::createProjectile(registry, Vector2{parentPosition.x, parentPosition.y}, Vector2{1, 0}, Parent{entity});
                } else {
                    std::cout << "Parent has no Position component" << std::endl;
                }
            }
        }
        ++it;
    }
}
