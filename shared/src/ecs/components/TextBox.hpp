/*
** TextBox.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 11:13:36 AM 2025 jojo
** Last update Wed Oct 7 8:37:36 PM 2025 jojo
*/

#pragma once
#include "Color.hpp"
#include <string>

/**
 * @brief Component that defines text display properties for UI text elements
 * Contains styling and positioning information for rendering text in the game UI
 */
struct TextBox {
    std::string text; /**< The text content to display */
    int fontSize = 16; /**< Font size in pixels */
    Color color = { 255, 255, 255, 255 }; /**< Text color (default: white) */
    std::string fontPath = "res/fonts/OpenSans-Medium.ttf"; /**< Path to the font file */
    bool visible = true; /**< Whether the text is visible */
    enum class Alignment { LEFT,
        CENTER,
        RIGHT } alignment
        = Alignment::LEFT; /**< Text alignment */
};
