/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Configuration Constants
*/

#pragma once

// Screen dimensions
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

// Jump and gravity constants
constexpr float JUMP_HEIGHT = 200.0f; // pixels
constexpr float JUMP_TIME = 0.3f; // seconds
constexpr float FALL_MULT = 2.0f;

constexpr float V0 = 2.0f * JUMP_HEIGHT / JUMP_TIME;
constexpr float GRAVITY_UP = 2.0f * JUMP_HEIGHT / (JUMP_TIME * JUMP_TIME);
constexpr float GRAVITY_DOWN = FALL_MULT * GRAVITY_UP;
constexpr float MAX_FALL = 800.0f;

// Platform system constants
constexpr float PLATFORM_WIDTH = 120.0f;
constexpr float PLATFORM_HEIGHT = 20.0f;
constexpr float PLATFORM_LEVELS[] = { 200.0f, 300.0f, 400.0f, 500.0f };
constexpr int PLATFORM_LEVELS_COUNT = 4;
constexpr int PLATFORM_VELOCITY_X = -15.0f;
constexpr int PLATFORM_VELOCITY_Y = 0.0f;

// PowerUp system constants
constexpr float POWERUP_WIDTH = 20.0f;
constexpr float POWERUP_HEIGHT = 20.0f;
constexpr float POWERUP_LIFETIME = 30.0f;

// Enemy system constants
constexpr float ENEMY_VELOCITY_X = -50.0f;
constexpr float ENEMY_VELOCITY_Y = 0.0f;
constexpr float ENEMY_WIDTH = 32.0f;
constexpr float ENEMY_HEIGHT = 32.0f;

constexpr float PLATFORM_MIN_DIST_X = 200.0f;
constexpr float PLATFORM_MAX_DIST_X = 270.0f;
constexpr float PLATFORM_CROSS_DIST_X = PLATFORM_WIDTH * 1.5f;
constexpr float PLATFORM_MAX_DIFF_Y = 100.0f;
