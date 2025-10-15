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
                auto weaponEntity = it.entity();
                if (registry.has<Position>(parent.parent)) {
                    const Position& parentPosition = registry.get<Position>(parent.parent);
                    factories::createProjectile(
                        registry,
                        Position{{parentPosition.v.x, parentPosition.v.y}},
                        Velocity{{500.0f, 0.0f}},
                        Damage{damage.value},
                        Hitbox{50.0f, 50.0f, 0.0f, 0.0f},
                        Parent{weaponEntity},
                        Lifetime{3.0f}
                    );
                } else {
                    std::cout << "Parent has no Position component" << std::endl;
                }
            }
        }
        ++it;
    }
}
