#pragma once

constexpr float JUMP_HEIGHT = 200.f; // px
constexpr float JUMP_TIME = 0.3f; // sec
constexpr float FALL_MULT = 2.0f;

constexpr float V0 = 2.f * JUMP_HEIGHT / JUMP_TIME;
constexpr float GRAVITY_UP = 2.f * JUMP_HEIGHT / (JUMP_TIME * JUMP_TIME);
constexpr float GRAVITY_DOWN = FALL_MULT * GRAVITY_UP;
constexpr float MAX_FALL = 800.f;

struct Jump {
    bool isJumping = false;
    bool canJump = true; // prevent infinite jump
};
