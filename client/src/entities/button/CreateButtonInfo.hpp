#pragma once
#include "../textbox/TextBox.hpp"
#include "CreateButton.hpp"
#include "Registry.hpp"
#include "components/AllComponents.hpp"
#include "components/ButtonInfo.hpp"

namespace factories {
Entity createButtonInfo(Registry& registry, float x, float y, float width, float height, const std::string& TextInfo, const std::string& actionId, bool interactable = true);
}