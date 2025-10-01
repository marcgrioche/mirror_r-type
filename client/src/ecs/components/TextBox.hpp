/*
** TextBox.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 11:13:36 AM 2025 jojo
** Last update Thu Oct 1 11:15:15 AM 2025 jojo
*/

#pragma once
#include <SDL.h>
#include <string>

struct TextBox {
    std::string text;
    int fontSize = 16;
    SDL_Color color = { 255, 255, 255, 255 }; // blanc par défaut
    std::string fontPath = "res/fonts/OpenSans-Medium.ttf";
    bool visible = true;
    // optionnel: alignment (left, center, right)
    enum class Alignment { LEFT,
        CENTER,
        RIGHT } alignment
        = Alignment::LEFT;
};
