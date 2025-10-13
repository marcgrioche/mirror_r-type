/*
** Color.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 2:39:23 PM 2025 jojo
** Last update Wed Oct 7 7:44:14 PM 2025 jojo
*/

#pragma once
#include <cstdint>

/**
 * @brief Component representing RGBA color values for rendering
 * @param a Alpha channel (transparency) 0-255
 * @param r Red channel 0-255
 * @param g Green channel 0-255
 * @param b Blue channel 0-255
 */
struct Color {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
