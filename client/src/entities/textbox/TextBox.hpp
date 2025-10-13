/*
** TextBox.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/textbox
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 11:02:34 AM 2025 jojo
** Last update Thu Oct 8 2:32:52 PM 2025 jojo
*/

#pragma once
#include "Registry.hpp"
#include "components/TextBox.hpp"

namespace factories {

/**
 * @brief Creates a text box entity for displaying text
 * @param registry The ECS registry to create the entity in
 * @param text The text content to display
 * @param x X position of the text box
 * @param y Y position of the text box
 * @param fontSize Font size for the text (default 16)
 * @param color Text color (default white)
 * @param alignement Text alignment (default LEFT)
 * @return The created entity
 */
Entity createTextBox(Registry& registry, const std::string& text,
    float x, float y, int fontSize = 16,
    Color color = { 255, 255, 255, 255 }, ::TextBox::Alignment alignement = ::TextBox::Alignment::LEFT);

} // namespace Factory
