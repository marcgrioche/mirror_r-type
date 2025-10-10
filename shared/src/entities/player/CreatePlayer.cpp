/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Player
*/
#include "CreatePlayer.hpp"

#include "../components/Frequency.hpp"
#include "../weapon/CreateWeapon.hpp"
#include "components/Dash.hpp"
#include "components/Dead.hpp"
#include "components/Health.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/PowerUp.hpp"
#include "components/PreviousPosition.hpp"
#include "components/RigidBody.hpp"
#include "components/Tags.hpp"
#include "components/Velocity.hpp"

namespace factories {
Entity createPlayer(Registry& registry)
{
    Entity player = registry.create_entity();
    registry.emplace<Position>(player, 50.0f, -500.0f);
    registry.emplace<PreviousPosition>(player, 50.0f, 480.0f);
    registry.emplace<Velocity>(player, 0.0f, 0.0f);
    registry.emplace<Health>(player, 100);
    registry.emplace<Hitbox>(player, 32.0f, 32.0f, 0.0f, 0.0f);
    registry.emplace<Dead>(player);
    registry.emplace<Dash>(player, Dash {});
    registry.emplace<PowerUp>(player);
    registry.emplace<PlayerTag>(player);
    registry.emplace<RigidBody>(player);
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
    registry.emplace<Dead>(player);
    registry.add<Dash>(player, Dash {});
    registry.add<PlayerTag>(player, PlayerTag {});
    registry.emplace<PowerUp>(player);
    registry.emplace<RigidBody>(player);
    createWeapon(registry, Parent { player });
    return player;
}

Entity createPlayer(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox, const TextBox& textbox)
{
    Entity player = registry.create_entity();
    registry.add<Position>(player, position);
    registry.add<PreviousPosition>(player, PreviousPosition { position.x, position.y });
    registry.add<Velocity>(player, Velocity { 0.0f, 0.0f });
    registry.add<Health>(player, health);
    registry.add<Hitbox>(player, hitbox);
    registry.emplace<Dead>(player);
    registry.add<Dash>(player, Dash {});
    registry.add<PlayerTag>(player, PlayerTag {});
    registry.add<TextBox>(player, textbox);
    registry.emplace<PowerUp>(player);
    registry.emplace<RigidBody>(player);
    createWeapon(registry, Parent { player });
    return player;
}
}
