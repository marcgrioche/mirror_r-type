/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Player Input State Component - Tracks player input intent for animation
*/

#pragma once

#include "../../../include/PlayerMovementState.hpp"

/**
 * @brief Component that tracks the player's current input state
 * Used to determine movement animations based on player intent rather than just physics
 */
struct PlayerInputState {
    bool leftPressed = false; /**< LEFT input currently active */
    bool rightPressed = false; /**< RIGHT input currently active */
    FacingDirection lastFacingDirection = FacingDirection::RIGHT; /**< Last known facing direction */
};
