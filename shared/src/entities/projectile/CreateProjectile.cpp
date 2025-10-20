/*
** CreateProjectile.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/projectil
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 1:12:28 PM 2025 jojo
** Last update Wed Sep 23 1:12:30 PM 2025 jojo
*/

#include "CreateProjectile.hpp"
#include "components/Tags.hpp"
#include "FunctionMotion.hpp"
#include "componentutils/FunctionMotionUtils.hpp"

namespace factories {
Entity createProjectile(Registry& registry,
    const Position& position,
    const Velocity& velocity,
    const Damage& damage,
    const Hitbox& hitbox,
    const Parent& parent,
    const Lifetime& lifetime)
{
    Entity projectile = registry.create_entity();
    registry.emplace<Position>(projectile, position);
    registry.emplace<Velocity>(projectile, velocity);
    registry.emplace<Damage>(projectile, damage);
    registry.emplace<Hitbox>(projectile, hitbox);
    registry.emplace<Parent>(projectile, parent);
    registry.emplace<Lifetime>(projectile, lifetime);
    registry.emplace<ProjectileTag>(projectile);
    return projectile;
}

Entity createProjectileTemplate(
    Registry& registry,
    const Velocity& velocity,
    const Damage& damage,
    const Hitbox& hitbox)
{
    // This entity is a blueprint: no Position/Lifetime/Parent so systems ignore it
    Entity tpl = registry.create_entity();
    registry.emplace<Velocity>(tpl, velocity);
    registry.emplace<Damage>(tpl, damage);
    registry.emplace<Hitbox>(tpl, hitbox);
    registry.emplace<ProjectileTag>(tpl);
    return tpl;
}
}
