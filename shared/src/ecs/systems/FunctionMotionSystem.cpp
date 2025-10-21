/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** FunctionMotionSystem
*/

#include "FunctionMotionSystem.hpp"
#include "FunctionMotion.hpp"
#include "Position.hpp"
#include "componentutils/VectorUtils.hpp"

void functionMotionSystem(Registry &reg, float dt)
{
    auto view = reg.view<FunctionMotion, Position>();

    for (auto&& [fm, pos] : view) {
        fm.time += dt;
        Vector2 offset = fm.function(fm.time, dt);
        if (fm.mode == MotionMode::Absolute) {
            pos.v = fm.origin.v + offset;
        } else {
            pos.v += offset;
        }
        std::cout << "POS: " << pos.v.x << ", " << pos.v.y << std::endl;
    }
}
