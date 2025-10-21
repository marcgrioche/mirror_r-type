#pragma once

#include "../../../include/Config.hpp"
#include "../../levels/Level.hpp"
#include "Dead.hpp"
#include "Entity.hpp"
#include "Hitbox.hpp"
#include "Platform.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "Tags.hpp"
#include "Velocity.hpp"
#include "entities/powerUp/CreatePowerUp.hpp"
#include <array>
#include <random>

namespace factories {

/**
 * @brief Creates a one-way platform that can be jumped through from below
 * @param registry The ECS registry to create the entity in
 * @param posx X position for the platform
 * @param posy Y position for the platform
 * @param velx X velocity for the platform (default: PLATFORM_VELOCITY_X)
 * @param vely Y velocity for the platform (default: PLATFORM_VELOCITY_Y)
 * @return The created one-way platform entity
 */
Entity createOneWayPlatform(Registry& registry, float posx, float posy, float velx = PLATFORM_VELOCITY_X, float vely = PLATFORM_VELOCITY_Y);

/**
 * @brief Creates a solid platform entity
 * @param registry The ECS registry to create the entity in
 * @param posx X position for the platform
 * @param posy Y position for the platform
 * @param velx X velocity for the platform (default: PLATFORM_VELOCITY_X)
 * @param vely Y velocity for the platform (default: PLATFORM_VELOCITY_Y)
 * @return The created platform entity
 */
Entity createPlatform(Registry& registry, float posx, float posy, float velx = PLATFORM_VELOCITY_X, float vely = PLATFORM_VELOCITY_Y);

/**
 * @brief Generates a specified quantity of random platforms
 * @param registry The ECS registry to create entities in
 * @param quantity Number of platforms to generate
 * @return Vector of created platform entities
 */
std::vector<Entity> generateRandomPlatforms(Registry& registry, int quantity, const Level* level = nullptr);

/**
 * @brief Regenerates a specified quantity of random platforms (replaces existing ones)
 * @param registry The ECS registry to create entities in
 * @param quantity Number of platforms to regenerate
 * @return Vector of newly created platform entities
 */
std::vector<Entity> reGenerateRandomPlatforms(Registry& registry, int quantity, const Level* level = nullptr);
}
