/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** ParallaxBackgroundSystem - Handles parallax background rendering
*/

#pragma once

#include "Registry.hpp"
#include <string>
#include <vector>

struct ParallaxLayer {
    std::string textureId;
    float scrollSpeed; // Speed multiplier relative to platform speed
    float offsetX; // Current horizontal offset
    float offsetY; // Current vertical offset (usually 0 for side-scrolling)
};

/**
 * @brief System for rendering parallax background layers
 * Layers scroll at different speeds to create depth effect
 */
void parallaxBackgroundSystem(Registry& registry);
