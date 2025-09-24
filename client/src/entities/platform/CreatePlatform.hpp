#pragma once

#include "Registry.hpp"
#include "Entity.hpp"

namespace factories {
    Entity createOneWayPlatform(Registry& registry, float posx, float posy);
    Entity createPlatform(Registry& registry, float posx, float posy);
}