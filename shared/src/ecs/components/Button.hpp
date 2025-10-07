/*
** Button.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Sep 29 1:02:08 PM 2025 jojo
** Last update Tue Oct 6 3:12:45 PM 2025 jojo
*/

#pragma once
#include <string>

// Component bouton
struct Button {
    std::string action_id;
    bool is_hovered = false;
    bool was_pressed = false;
    bool interactable = true;
    Color bg = { 255, 255, 255, 255 };
    Color border = { 0, 0, 0, 0 };
};