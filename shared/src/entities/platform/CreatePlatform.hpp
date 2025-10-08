#pragma once

#include "Entity.hpp"
#include "Registry.hpp"
#include "entities/powerUp/CreatePowerUp.hpp"
#include "Dead.hpp"
#include "Hitbox.hpp"
#include "Platform.hpp"
#include "Position.hpp"
#include "Tags.hpp"
#include "Velocity.hpp"
#include <array>
#include <random>

namespace factories {
Entity createOneWayPlatform(Registry& registry, float posx, float posy);
Entity createPlatform(Registry& registry, float posx, float posy);
std::vector<Entity> generateRandomPlatforms(Registry& registry, int quantity);
std::vector<Entity> reGenerateRandomPlatforms(Registry& registry, int quantity);
}