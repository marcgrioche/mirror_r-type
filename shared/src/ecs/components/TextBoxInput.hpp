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

/**
 * @brief Component that manages interactive text input fields for UI
 * Handles user input, cursor positioning, focus states, and visual feedback for text input widgets
 */
struct TextBoxInput {
    std::string inputText = ""; /**< Text entered by the user */
    std::string placeholder = "Enter text..."; /**< Placeholder text shown when input is empty */
    bool isFocused = false; /**< Whether the text box currently has input focus */
    bool isActive = true; /**< Whether the text box can receive input */
    size_t maxLength = 100; /**< Maximum number of characters allowed */
    size_t cursorPosition = 0; /**< Current cursor position in the text */
    float cursorBlinkTimer = 0.0f; /**< Timer for cursor blinking animation */
    bool showCursor = true; /**< Current cursor visibility state for blinking */
    Color focusColor = { 100, 150, 255, 255 }; /**< Border color when text box has focus */
    Color normalColor = { 150, 150, 150, 255 }; /**< Border color when text box is not focused */
};
