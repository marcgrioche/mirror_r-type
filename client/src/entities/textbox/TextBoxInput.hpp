/*
** TextBoxInput.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/textbox
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 2:15:59 PM 2025 jojo
** Last update Thu Oct 8 2:32:33 PM 2025 jojo
*/

#pragma once
#include "Registry.hpp"
#include "components/TextBox.hpp"
#include "components/TextBoxInput.hpp"

namespace factories {

/**
 * @brief Creates a text box input entity for user text input
 * @param registry The ECS registry to create the entity in
 * @param placeOlder Placeholder text to display when input is empty
 * @param x X position of the text box
 * @param y Y position of the text box
 * @param fontSize Font size for the text (default 16)
 * @param color Text color (default white)
 * @return The created entity
 */
Entity createTextBoxInput(Registry& registry, const std::string& placeOlder,
    float x, float y, int fontSize = 16,
    Color color = { 255, 255, 255, 255 });

} // namespace Factory
