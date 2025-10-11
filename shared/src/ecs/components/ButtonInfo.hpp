/*
** ButtonInfo.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 2:13:11 PM 2025 jojo
** Last update Thu Oct 1 2:14:46 PM 2025 jojo
*/
#pragma once
#include "Registry.hpp"

/**
 * @brief Component that provides tooltip information for buttons
 * Contains references to tooltip text boxes and display settings for button help text
 */
struct ButtonInfo {
    Entity tooltipEntity; /**< Reference to the text box entity that displays the tooltip */
    std::string infoText; /**< The text content of the tooltip */
    bool showOnHover = true; /**< Whether to show the tooltip when hovering over the button */
};
