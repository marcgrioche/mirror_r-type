/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Player
*/
#include "CreatePlayer.hpp"
#include "../weapon/CreateWeapon.hpp"

namespace factories {
Entity createPlayer(Registry& registry)
{
    Entity player = registry.create_entity();
    registry.emplace<Position>(player, 50.0f, -500.0f);
    registry.emplace<PreviousPosition>(player, 50.0f, 480.0f);
    registry.emplace<Velocity>(player, 0.0f, 0.0f);
    registry.emplace<Health>(player, 100);
    registry.emplace<Hitbox>(player, 32.0f, 32.0f, 0.0f, 0.0f);
    registry.emplace<Jump>(player);
    registry.emplace<Dash>(player, Dash { 0.2f, 0.0f, false, 800.0f, Frequency { 0.5 }, 1 });
    registry.emplace<PowerUp>(player);
    registry.emplace<PlayerTag>(player);
    // registry.emplace<Sprite>(player, 0, 50, 50);
    createWeapon(registry, Parent { player });
    return player;
}

Entity createPlayer(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox)
{
    Entity player = registry.create_entity();
    registry.add<Position>(player, position);
    registry.add<PreviousPosition>(player, PreviousPosition { position.x, position.y });
    registry.add<Velocity>(player, Velocity { 0.0f, 0.0f });
    registry.add<Health>(player, health);
    registry.add<Hitbox>(player, hitbox);
    registry.add<Jump>(player, Jump {});
    registry.add<Dash>(player, Dash { 0.2f, 0.0f, false, 800.0f, Frequency { 0.5 }, 1 });
    registry.add<PlayerTag>(player, PlayerTag {});
    registry.emplace<PowerUp>(player);
    createWeapon(registry, Parent { player });
    return player;
}
}
