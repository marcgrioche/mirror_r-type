/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** FunctionMotion
*/

#pragma once
#include <functional>
#include "Vector2.hpp"

//strictly follows the function curve, so it cancels any velocity
struct FunctionMotion {
    std::function<Vector2(float)> function;
    float time = 0.0f;
    Vector2 origin;
};
