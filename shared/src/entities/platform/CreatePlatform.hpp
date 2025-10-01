#pragma once

#include "Entity.hpp"
#include "Registry.hpp"
#include "components/AllComponents.hpp"

namespace factories {
Entity createOneWayPlatform(Registry& registry, float posx, float posy);
Entity createPlatform(Registry& registry, float posx, float posy);
Entity createPlatform(Registry& registry, const Position& position, const Hitbox& hitbox);
}
