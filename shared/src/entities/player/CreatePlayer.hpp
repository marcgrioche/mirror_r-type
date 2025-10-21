/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Player
*/

#pragma once
#include "Config.hpp"
#include "Registry.hpp"
#include "components/Health.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"
#include "components/TextBox.hpp"

namespace factories {

/**
 * @brief Creates a player entity with customizable components
 * @param registry The ECS registry to create the entity in
 * @param username The player's username (defaults to empty string for TextBox display)
 * @param position Initial position (defaults to 50, -500)
 * @param health Health component (defaults to 100 HP)
 * @param hitbox Hitbox component (defaults to 32x32)
 * @param playerId The player ID for the PlayerTag component (defaults to 0)
 * @return The created player entity
 */
Entity createPlayer(
    Registry& registry,
    const std::string& username = "",
    const Position& position = Position { 50.0f, -500.0f },
    const Health& health = Health { 100 },
    const Hitbox& hitbox = Hitbox { 32.0f, 32.0f, 0.0f, 0.0f },
    uint32_t playerId = 0);
}
