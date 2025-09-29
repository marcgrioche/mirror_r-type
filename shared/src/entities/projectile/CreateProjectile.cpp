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

namespace factories {
void createProjectile(Registry& registry, Vector2 position, Vector2 velocity,
    Entity owner, float lifetime, int damage)
{
    Entity projectile = registry.create_entity();
    registry.emplace<Position>(projectile, position.x, position.y);
    registry.emplace<Velocity>(projectile, velocity.x, velocity.y);
    registry.emplace<Damage>(projectile, static_cast<float>(damage));
    registry.emplace<Hitbox>(projectile, 50.0f, 50.0f, 0.0f, 0.0f);
    registry.emplace<OwnerId>(projectile, static_cast<int>(owner.id));
    registry.emplace<Lifetime>(projectile, lifetime);
    registry.emplace<Projectile>(projectile);
    // registry.emplace<Sprite>(projectile, 0, 50, 50);
}

void createProjectile(Registry& registry, const Position& position, const Velocity& velocity,
    const Damage& damage, const Hitbox& hitbox, const OwnerId& ownerId, const Lifetime& lifetime)
{
    Entity projectile = registry.create_entity();
    registry.add<Position>(projectile, position);
    registry.add<Velocity>(projectile, velocity);
    registry.add<Damage>(projectile, damage);
    registry.add<Hitbox>(projectile, hitbox);
    registry.add<OwnerId>(projectile, ownerId);
    registry.add<Lifetime>(projectile, lifetime);
    registry.add<Projectile>(projectile, Projectile {});
}
}
