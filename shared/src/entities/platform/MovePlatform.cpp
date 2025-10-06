/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** HandlePlayerInputs
*/

#include "MovePlatform.hpp"
#include <iostream>

void movePlatform(Registry& registry)
{
    auto view = registry.view<PlatformTag, Velocity>();
    const float speed = 30.0f;

    for (auto [tag, velocity] : view) {
        velocity.dx =  0.0f;
        velocity.dx = -speed;
    }
}