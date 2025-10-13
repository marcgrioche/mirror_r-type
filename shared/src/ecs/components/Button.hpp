/*
** Button.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Sep 29 1:02:08 PM 2025 jojo
** Last update Wed Oct 7 7:44:27 PM 2025 jojo
*/

#pragma once
#include "Color.hpp"
#include <string>

/**
 * @brief Component that defines button properties for UI interaction
 * Contains visual and interaction state information for clickable UI buttons
 */
struct Button {
    std::string action_id; /**< Unique identifier for the button's action */
    bool is_hovered = false; /**< Whether the mouse cursor is currently over the button */
    bool was_pressed = false; /**< Whether the button was pressed in the last frame */
    bool interactable = true; /**< Whether the button can be interacted with */
    Color bg { 255, 255, 255, 255 }; /**< Background color of the button (default: white) */
    Color border { 0, 0, 0, 0 }; /**< Border color of the button (default: transparent) */
};
