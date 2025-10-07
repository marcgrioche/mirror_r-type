/*
** CreateButton.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/entities/button
**
** Made by jojo
** Login   <jojo>
**
** Started on  Mon Sep 29 1:40:18 PM 2025 jojo
** Last update Tue Sep 29 1:40:29 PM 2025 jojo
*/

#pragma once
#include "Registry.hpp"
#include "components/AllComponents.hpp"

namespace factories {
Entity createButton(std::shared_ptr<Registry> registry, float x, float y, float width, float height,
    const std::string& actionId, bool interactable = true);
}
