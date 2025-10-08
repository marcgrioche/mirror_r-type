/*
** TextBox.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/textbox
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 11:02:34 AM 2025 jojo
** Last update Thu Oct 8 12:45:11 PM 2025 jojo
*/

#pragma once
#include "Registry.hpp"
#include "components/AllComponents.hpp"
#include "components/TextBox.hpp"

namespace factories {
Entity createTextBox(Registry& registry, const std::string& text,
    float x, float y, int fontSize = 16,
    Color color = { 255, 255, 255, 255 }, ::TextBox::Alignment alignement = ::TextBox::Alignment::LEFT);

} // namespace Factory
