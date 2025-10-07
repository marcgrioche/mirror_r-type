#pragma once

#include "Entity.hpp"
#include "Registry.hpp"

namespace factories {
Entity createOneWayPlatform(std::shared_ptr<Registry> registry, float posx, float posy);
Entity createPlatform(std::shared_ptr<Registry> registry, float posx, float posy);
std::vector<Entity> generateRandomPlatforms(std::shared_ptr<Registry> registry, int quantity);
}