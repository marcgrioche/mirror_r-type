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

void handleWeaponInputs(InputManager& _inputs, Registry& registry, float deltaTime)
{
    auto view = registry.view<WeaponTag, Magazine, Damage, Frequency, Position>();

    for (auto it = view.begin(); it != view.end();) {
        auto [w, magazine, damage, frequency, position] = *it;
        if (_inputs.isShoot()) {
            if (frequency.lastTime >= frequency.seconds) {
                auto entity = it.entity();
                factories::createProjectile(registry, Vector2{position.x, position.y}, Vector2{1, 0}, Parent{entity});
            }
        }
    }
}
