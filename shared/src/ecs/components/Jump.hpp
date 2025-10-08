#pragma once

#include "../../include/Config.hpp"

struct Jump {
    bool isJumping = false;
    bool canJump = true;

    bool dropping = false;
    float dropTimer = 0.0f;
};
