#pragma once

#include <cstdint>

/**
 * @brief Enumeration of game input actions
 */
enum class GameInput : uint8_t {
    UP = 0, /**< Move up input */
    DOWN = 1, /**< Move down input */
    LEFT = 2, /**< Move left input */
    RIGHT = 3, /**< Move right input */
    ATTACK = 4, /**< Attack/shoot input */
    DASH = 5 /**< Dash input */
};
