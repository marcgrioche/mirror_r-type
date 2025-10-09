/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** CreateWeapon
*/

#include "CreateWeapon.hpp"
#include "Damage.hpp"
#include "Magazine.hpp"
#include "Frequency.hpp"
#include "ProjectileType.hpp"
#include "Tags.hpp"

Entity factories::createWeapon(Registry &registry)
{
    Entity weapon = registry.create_entity();
    registry.emplace<WeaponTag>(weapon);
    registry.emplace<Magazine>(weapon, Magazine{30, 30});
    registry.emplace<Damage>(weapon, Damage{10});
    registry.emplace<Frequency>(weapon, Frequency{1});
    return weapon;
}

Entity factories::createWeapon(Registry &registry, Parent parent, Entity projectile)
{
    Entity weapon = createWeapon(registry);
    registry.emplace<Parent>(weapon, parent);
    registry.emplace<ProjectileType>(weapon, projectile);
    return weapon;
}
