/*
** TextBox.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/entities/textbox
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 11:02:34 AM 2025 jojo
** Last update Thu Oct 1 11:21:05 AM 2025 jojo
*/

#pragma once
#include "../../components/TextBox.hpp"
#include "Registry.hpp"
#include "components/AllComponents.hpp"

namespace Factory {
Entity TextBox(Registry& registry, const std::string& text,
    float x, float y, int fontSize = 16,
    SDL_Color color = { 255, 255, 255, 255 });

} // namespace Factory
