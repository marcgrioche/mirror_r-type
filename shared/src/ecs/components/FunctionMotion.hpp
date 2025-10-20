/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** FunctionMotion
*/

#pragma once
#include <functional>
#include "Vector2.hpp"
#include "Position.hpp"

//absolute is based on the origin, and relative is based on the last frame position
//the function have to be adapted depending on the MotionMode
//prefer relative for easier weapon template projectile handling
enum class MotionMode {Absolute, Relative};

struct FunctionMotion {
    std::function<Vector2(float, float)> function;
    float time = 0.0f;
    MotionMode mode = MotionMode::Relative;
    Position origin; //for Absolute only
};
