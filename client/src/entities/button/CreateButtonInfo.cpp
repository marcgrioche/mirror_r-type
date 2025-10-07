/*
** CreateButtonInfo.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/button
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 10:46:09 AM 2025 jojo
** Last update Wed Oct 7 7:56:56 PM 2025 jojo
*/

#include "CreateButtonInfo.hpp"

namespace factories {
Entity createButtonInfo(Registry& registry, float x, float y, float width, float height, const std::string& TextInfo, const std::string& actionId, bool interactable)
{
    Entity button = createButton(registry, x, y, width, height, actionId, interactable);

    Entity textBox = createTextBox(registry, TextInfo, x, y);

    if (registry.has<::TextBox>(textBox)) {
        registry.get<::TextBox>(textBox).visible = false;
    }

    registry.emplace<ButtonInfo>(button, textBox, TextInfo, true);

    return button;
}
}