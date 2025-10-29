/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Player Movement State and Facing Direction Enums
*/

#pragma once

/**
 * @brief Enumeration of player movement states for animation synchronization
 * Used to determine which animation row to display in the sprite sheet
 */
enum class PlayerMovementState {
    IDLE = 0, /**< No horizontal input active (may have platform-induced velocity) */
    RUNNING = 1, /**< Player actively pressing LEFT/RIGHT */
    DASHING = 2, /**< Currently dashing */
    JUMPING = 3 /**< In air (not on ground) */
};

/**
 * @brief Enumeration of player facing directions for sprite flipping
 */
enum class FacingDirection {
    LEFT = 0, /**< Facing left (sprite should be flipped) */
    RIGHT = 1 /**< Facing right (normal orientation) */
};
