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
#include "components/Button.hpp"
#include "components/Hitbox.hpp"
#include "components/Position.hpp"

namespace factories {

/**
 * @brief Creates a button entity with specified properties
 * @param registry The ECS registry to create the entity in
 * @param x X position of the button
 * @param y Y position of the button
 * @param width Width of the button
 * @param height Height of the button
 * @param actionId Identifier for the button's action
 * @param interactable Whether the button can be interacted with (default true)
 * @return The created entity
 */
Entity createButton(Registry& registry, float x, float y, float width, float height,
    const std::string& actionId, bool interactable = true);
}
