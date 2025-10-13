#pragma once
#include "../textbox/TextBox.hpp"
#include "CreateButton.hpp"
#include "Registry.hpp"
#include "components/ButtonInfo.hpp"

namespace factories {

/**
 * @brief Creates a button info entity with specified properties
 * @param registry The ECS registry to create the entity in
 * @param x X position of the button
 * @param y Y position of the button
 * @param width Width of the button
 * @param height Height of the button
 * @param TextInfo Text to display on the button
 * @param actionId Identifier for the button's action
 * @param interactable Whether the button can be interacted with (default true)
 * @return The created entity
 */
Entity createButtonInfo(Registry& registry, float x, float y, float width, float height, const std::string& TextInfo, const std::string& actionId, bool interactable = true);
}
