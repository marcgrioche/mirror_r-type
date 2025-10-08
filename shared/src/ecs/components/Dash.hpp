/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Dash Component - Handles player dash ability
*/

#pragma once
#include "Frequency.hpp"
#include "Vector2.hpp"

struct Dash {
    float duration = 0.08f;
    float remaining = 0.0f;
    bool isDashing = false;
    float dashSpeed = 1000.0f;
    Frequency cooldown = Frequency {0.5};
    Vector2 direction = {0, 0};
};
