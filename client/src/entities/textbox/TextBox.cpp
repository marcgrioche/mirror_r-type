/*
** TextBox.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/textbox
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 11:03:28 AM 2025 jojo
** Last update Thu Oct 1 2:08:31 PM 2025 jojo
*/

#include "TextBox.hpp"

namespace factories {

Entity TextBox(Registry& registry, const std::string& text,
    float x, float y, int fontSize, SDL_Color color)
{
    Entity textBox = registry.create_entity();

    registry.emplace<Position>(textBox, x, y);
    registry.emplace<TextBox>(textBox, text, fontSize, color,
        "assets/fonts/default.ttf", true,
        ::TextBox::Alignment::LEFT);

    return textBox;
}

}