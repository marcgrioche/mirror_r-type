/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** FunctionMotionUtils
*/

#pragma once
#include "FunctionMotion.hpp"
#include "Position.hpp"
#include <cmath>

FunctionMotion getFunctionMotionSineAbsolute(Position origin, float vx = 300.f, float frequency = 3.f, float amplitude = 100.f)
{
    FunctionMotion fm = {
        .function = [vx, frequency, amplitude](float t, float dt) {
            (void)dt;
            float x = vx * t;
            float y = std::sin(t * frequency * 2.f * M_PI) * amplitude;
            return Vector2 { x, y };
        },
        .mode = MotionMode::Absolute,
        .origin = origin
    };
    return fm;
}

FunctionMotion getFunctionMotionSineRelative(float vx = 400.f, float frequency = 0.4f, float amplitude = 400.f)
{
    FunctionMotion fm = {
        .function = [vx, frequency, amplitude](float t, float dt) {
            float vy = std::cos(t * frequency * 2.f * M_PI) * amplitude * frequency * 2.f * M_PI;
            return Vector2 { vx * dt, vy * dt };
        },
        .mode = MotionMode::Relative
    };
    return fm;
}
