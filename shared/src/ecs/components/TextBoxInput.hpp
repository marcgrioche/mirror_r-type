/*
** TextBoxInput.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 2:16:36 PM 2025 jojo
** Last update Wed Oct 7 2:25:39 PM 2025 jojo
*/

#pragma once
#include "Color.hpp"
#include <string>

struct TextBoxInput {
    std::string inputText = ""; // texte saisi par l'utilisateur
    std::string placeholder = "Enter text..."; // texte d'aide
    bool isFocused = false; // si la textbox a le focus
    bool isActive = true; // si elle peut recevoir input
    size_t maxLength = 100; // limite de caractères
    size_t cursorPosition = 0; // position du curseur
    float cursorBlinkTimer = 0.0f; // pour animation curseur
    bool showCursor = true; // état clignotement
    Color focusColor = { 100, 150, 255, 255 }; // couleur quand focus
    Color normalColor = { 150, 150, 150, 255 }; // couleur normale
};