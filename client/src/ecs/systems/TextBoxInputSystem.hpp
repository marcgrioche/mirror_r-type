/*
** TextBoxInputSystem.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 2:46:05 PM 2025 jojo
** Last update Wed Oct 7 3:14:55 PM 2025 jojo
*/

#pragma once

#include "Registry.hpp"
#include "components/AllComponents.hpp"
#include "components/TextBoxInput.hpp"
#include <SDL2/SDL.h>

// Système qui gère les événements clavier pour les TextBoxInput
void textBoxInputSystem(Registry& registry, const SDL_Event& event);

// Système qui met à jour l'affichage et le curseur
void textBoxInputUpdateSystem(Registry& registry, float deltaTime);
// ...existing code...