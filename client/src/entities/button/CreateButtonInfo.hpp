#pragma once
#include "CreateButton.hpp"
#include "Registry.hpp"
#include "components/AllComponents.hpp"

namespace factories {
Entity createButtonInfo(Registry& registry, float x, float y, float width, float height,
    const std::string& actionId, bool interactable = true);
}