/*
** TextBox.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Wed Oct 1 11:13:36 AM 2025 jojo
** Last update Wed Oct 7 8:37:36 PM 2025 jojo
*/

#pragma once
#include "Color.hpp"
#include <string>

struct TextBox {
    std::string text;
    int fontSize = 16;
    Color color = { 255, 255, 255, 255 }; // blanc par défaut
    std::string fontPath = "client/res/fonts/OpenSans-Medium.ttf";
    bool visible = true;
    enum class Alignment { LEFT,
        CENTER,
        RIGHT } alignment
        = Alignment::LEFT;
};
