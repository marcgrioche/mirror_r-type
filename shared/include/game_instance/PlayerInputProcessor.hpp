#pragma once

#include "../../src/ecs/Entity.hpp"
#include "../../src/ecs/Registry.hpp"
#include "../GameInputs.hpp"

namespace PlayerInputProcessor {

bool processInput(
    Registry& registry,
    Entity playerEntity,
    uint32_t tick,
    const std::vector<std::pair<GameInput, bool>>& inputs,
    std::vector<Entity>& newEntities,
    uint32_t playerId,
    bool isClient = false);

}
