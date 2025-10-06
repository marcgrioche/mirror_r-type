/*
** CreateButtonInfo.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/button
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 10:46:09 AM 2025 jojo
** Last update Thu Oct 1 2:22:53 PM 2025 jojo
*/

#include "CreateButtonInfo.hpp"

namespace factories {
Entity createButtonInfo(Registry& registry, float x, float y, float width, float height,
    const std::string& textInfo, const std::string& actionId, bool interactable = true)
{
    Entity button = createButton(registry, x, y, width, height, actionId, interactable);

    Entity textBox = createTextBox(registry, textInfo, x, y);

    if (registry.has<::TextBox>(textBox)) {
        registry.get<::TextBox>(textBox).visible = false;
    }

    registry.emplace<ButtonInfo>(button, textBox, textInfo, true);

    return button;
}
}