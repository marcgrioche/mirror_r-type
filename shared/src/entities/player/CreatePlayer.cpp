/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Player
*/
#include "CreatePlayer.hpp"
#include "../projectile/CreateProjectile.hpp"
#include "../weapon/CreateWeapon.hpp"
#include "components/Health.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/PreviousPosition.hpp"
#include "components/Velocity.hpp"
// #include "components/Jump.hpp"
#include "../components/Frequency.hpp"
#include "components/Damage.hpp"
#include "components/Dash.hpp"
#include "components/Dead.hpp"
#include "components/Lifetime.hpp"
#include "components/PowerUp.hpp"
#include "components/RigidBody.hpp"
#include "components/Tags.hpp"

namespace factories {

Entity createPlayer(
    Registry& registry,
    const std::string& username,
    const Position& position,
    const Health& health,
    const Hitbox& hitbox,
    uint32_t playerId)
{
    Entity player = registry.create_entity();
    registry.emplace<Position>(player, position.v.x, position.v.y);
    registry.emplace<PreviousPosition>(player, position.v.x, position.v.y);
    registry.emplace<Velocity>(player, 0.0f, 0.0f);
    registry.emplace<Health>(player, health.hp);
    registry.emplace<Hitbox>(player, hitbox.width, hitbox.height, hitbox.offset_x, hitbox.offset_y);
    registry.emplace<Dead>(player);
    registry.emplace<Dash>(player, Dash {});
    registry.emplace<PowerUp>(player);
    registry.emplace<PlayerTag>(player, PlayerTag { playerId });
    registry.emplace<RigidBody>(player);
    registry.emplace<TextBox>(player, username, 12);

    Entity projectile = factories::createProjectileTemplate(
        registry,
        Velocity { 1000, 0 },
        Damage { 5 },
        Hitbox { 25, 25 });
    createWeapon(registry, Parent { player }, projectile);
    return player;
}
}
