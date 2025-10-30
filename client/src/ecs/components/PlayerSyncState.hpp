/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Player Sync State Component - Client-side storage for server-synced movement state
*/

#pragma once

#include "../../../shared/include/PlayerMovementState.hpp"

/**
 * @brief Component that stores the player's movement state and facing direction received from server
 * Used by the animation and rendering systems to display the correct sprite state
 */
struct PlayerSyncState {
    PlayerMovementState movementState = PlayerMovementState::IDLE;
    FacingDirection facingDirection = FacingDirection::RIGHT;
};
