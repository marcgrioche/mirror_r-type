#pragma once

#include <cstdint>

enum class GameInput : uint8_t {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
    ATTACK = 4,
    DASH = 5
};
