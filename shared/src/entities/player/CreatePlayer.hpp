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
 * @brief Creates a default player entity with standard components
 * @param registry The ECS registry to create the entity in
 * @return The created player entity
 */
Entity createPlayer(Registry& registry);

/**
 * @brief Creates a player entity with specified position, health, and hitbox
 * @param registry The ECS registry to create the entity in
 * @param position The initial position component for the player
 * @param health The health component for the player
 * @param hitbox The hitbox component for collision detection
 * @return The created player entity
 */
Entity createPlayer(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox);

/**
 * @brief Creates a player entity with position, health, hitbox, and textbox components
 * @param registry The ECS registry to create the entity in
 * @param position The initial position component for the player
 * @param health The health component for the player
 * @param hitbox The hitbox component for collision detection
 * @param textbox The textbox component for player name display
 * @param playerId The player id
 * @return The created player entity
 */
Entity createPlayer(Registry& registry, const Position& position, const Health& health, const Hitbox& hitbox, const TextBox& textbox, uint32_t playerId);
}
