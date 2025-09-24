/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Player
*/
#include "CreatePlayer.hpp"
#include "../weapon/CreateWeapon.hpp"

namespace factories {
void createPlayer(Registry& registry)
{
    Entity player = registry.create_entity();
    registry.emplace<Position>(player, 50.0f, -500.0f);
    registry.emplace<PreviousPosition>(player, 50.0f, 480.0f);
    registry.emplace<Velocity>(player, 0.0f, 0.0f);
    registry.emplace<Health>(player, 100);
    registry.emplace<Hitbox>(player, 32.0f, 32.0f, 0.0f, 0.0f);
    registry.emplace<Jump>(player);
    registry.emplace<PlayerTag>(player);
    // registry.emplace<Sprite>(player, 0, 50, 50);
    createWeapon(registry, Parent{player});
}
}
