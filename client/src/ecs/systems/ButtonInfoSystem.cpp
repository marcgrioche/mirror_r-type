/*
** ButtonInfoSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 2:30:59 PM 2025 jojo
** Last update Thu Oct 8 2:39:02 PM 2025 jojo
*/

#include "ButtonInfoSystem.hpp"
#include "Button.hpp"
#include "ButtonInfo.hpp"
#include "Position.hpp"
#include "components/TextBox.hpp"

void buttonInfoSystem(Registry& registry)
{
    auto view = registry.view<Button, ButtonInfo, Position>();

    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity buttonEntity = it.entity();

        if (!registry.has<Button>(buttonEntity) || !registry.has<ButtonInfo>(buttonEntity) || !registry.has<Position>(buttonEntity)) {
            continue;
        }

        const Button& button = registry.get<Button>(buttonEntity);
        const ButtonInfo& btnInfo = registry.get<ButtonInfo>(buttonEntity);
        const Position& btnPos = registry.get<Position>(buttonEntity);

        // Vérifie si le tooltip existe toujours
        Entity tooltipEntity = btnInfo.tooltipEntity;
        if (!registry.has<TextBox>(tooltipEntity) || !registry.has<Position>(tooltipEntity)) {
            continue;
        }

        TextBox& tooltip = registry.get<TextBox>(tooltipEntity);
        Position& tooltipPos = registry.get<Position>(tooltipEntity);

        // Affiche/cache le tooltip selon l'état hover du bouton
        if (button.is_hovered && btnInfo.showOnHover) {
            tooltip.visible = true;
        } else {
            tooltip.visible = false;
        }
    }
}