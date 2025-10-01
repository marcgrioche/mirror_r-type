/*
** CreateButton.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/entities/button
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Sep 29 1:41:01 PM 2025 jojo
** Last update Tue Sep 29 1:41:08 PM 2025 jojo
*/

#include "CreateButton.hpp"

namespace factories {

Entity createButton(Registry& registry, float x, float y, float width, float height,
    const std::string& actionId, bool interactable)
{
    Entity button = registry.create_entity();

    // Position sur l'écran
    registry.emplace<Position>(button, x, y);

    // Zone cliquable
    registry.emplace<Hitbox>(button, width, height, 0.0f, 0.0f);

    // State et action du bouton
    registry.emplace<Button>(button, actionId, false, false, interactable);

    // Optionnel : sprite pour le rendu
    // registry.emplace<Sprite>(button, "button_texture",
    //                         SDL_Rect{0, 0, 64, 32},
    //                         SDL_Rect{(int)x, (int)y, (int)width, (int)height});

    return button;
}

}