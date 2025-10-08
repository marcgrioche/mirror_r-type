#pragma once

#include "../../include/Config.hpp"

struct Jump {
    bool isJumping = false;
    bool canJump = true; // prevent infinite jump
    // Drop-through one-way platform support
    bool dropping = false;      // true while player intentionally passes through a one-way platform
    float dropTimer = 0.0f;     // remaining time window during which collisions with one-way platforms are ignored
};
