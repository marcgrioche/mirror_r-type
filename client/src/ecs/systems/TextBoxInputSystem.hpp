/*
** TextBoxInputSystem.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 2:46:05 PM 2025 jojo
** Last update Thu Oct 8 2:32:08 PM 2025 jojo
*/

#pragma once

#include "Registry.hpp"
#include "components/TextBoxInput.hpp"
#include <SDL2/SDL.h>

/**
 * @brief System that handles keyboard input events for text box components
 * Processes SDL keyboard events to update text input fields and handle text editing
 * @param registry The ECS registry containing all game entities and components
 * @param event The SDL event containing keyboard input information
 */
void textBoxInputSystem(Registry& registry, const SDL_Event& event);

/**
 * @brief System that updates text box display and cursor animation
 * Manages cursor blinking and text display updates for input text boxes
 * @param registry The ECS registry containing all game entities and components
 * @param deltaTime Time elapsed since the last frame in seconds
 */
void textBoxInputUpdateSystem(Registry& registry, float deltaTime);
// ...existing code...
