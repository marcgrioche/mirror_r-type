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
Entity createTextBoxInput(Registry& registry, const std::string& placeOlder,
    float x, float y, int fontSize = 16,
    Color color = { 255, 255, 255, 255 });

} // namespace Factory
