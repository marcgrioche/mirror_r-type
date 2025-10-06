#pragma once

#include "Entity.hpp"
#include "Registry.hpp"

namespace factories {
Entity createOneWayPlatform(Registry& registry, float posx, float posy);
Entity createPlatform(Registry& registry, float posx, float posy);
std::vector<Entity> generateRandomPlatforms(Registry& registry, int quantity);
std::vector<Entity> reGenerateRandomPlatforms(Registry& registry, int quantity);
}