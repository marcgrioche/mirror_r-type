#pragma once

#include "../../include/Config.hpp"

struct Jump {
    bool isJumping = false;
    bool canJump = true; // prevent infinite jump
};
