/*
** ButtonInfo.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 2:13:11 PM 2025 jojo
** Last update Thu Oct 1 2:14:46 PM 2025 jojo
*/
#include "Registry.hpp"
#pragma once

struct ButtonInfo {
    Entity tooltipEntity; // référence vers la TextBox d'info
    std::string infoText; // texte du tooltip
    bool showOnHover = true;
};
