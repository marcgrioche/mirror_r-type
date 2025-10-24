#include "../../include/Config.hpp"
#include "../../include/game_instance/GameInstancePlayer.hpp"
#include "../ecs/components/Dash.hpp"
#include "../ecs/components/RigidBody.hpp"
#include "../ecs/components/Velocity.hpp"
#include "../ecs/systems/FrequencyUtils.hpp"
#include "../ecs/systems/WeaponSystem.hpp"

namespace PlayerInputProcessor {

bool processInput(
    Registry& registry,
    Entity playerEntity,
    uint32_t tick,
    const std::vector<std::pair<GameInput, bool>>& inputs,
    std::vector<Entity>& newEntities,
    uint32_t playerId,
    bool isClient,
    float mouseX,
    float mouseY)
{
    (void)tick;

    if (!registry.has<Velocity>(playerEntity) || !registry.has<RigidBody>(playerEntity) || !registry.has<Dash>(playerEntity)) {
        return false;
    }

    auto& velocity = registry.get<Velocity>(playerEntity);
    auto& dash = registry.get<Dash>(playerEntity);
    auto& rb = registry.get<RigidBody>(playerEntity);

    if (!dash.isDashing) {
        rb.acceleration = { 0.0f, 0.0f };
        dash.direction = { 0.0f, 0.0f };
    }

    bool hasRealInputs = false;
    float accel = rb.isOnGround ? rb.groundAccel : rb.airAccel;

    for (const auto& [input, isPressed] : inputs) {
        if (!isPressed)
            continue;
        hasRealInputs = true;

        switch (input) {
        case GameInput::UP:
            if (!dash.isDashing) {
                if (rb.isOnGround) {
                    velocity.v.y = -V0;
                }
                dash.direction.y = -1;
            }
            break;
        case GameInput::DOWN:
            if (!dash.isDashing) {
                dash.direction.y = 1;
            }
            break;
        case GameInput::LEFT:
            if (!dash.isDashing) {
                rb.acceleration.x = -accel;
                dash.direction.x = -1;
            }
            break;
        case GameInput::RIGHT:
            if (!dash.isDashing) {
                rb.acceleration.x = accel;
                dash.direction.x = 1;
            }
            break;
        case GameInput::ATTACK:
            if (isClient)
                continue;
            // Pass mouse coordinates to weapon system for aiming
            if (WeaponSystem::handlePlayerAttack(registry, playerEntity, playerId, newEntities, mouseX, mouseY)) {
                // Attack processed with mouse aiming
            }
            break;
        case GameInput::DASH:
            if (!dash.isDashing && FrequencyUtils::shouldTrigger(dash.cooldown)) {
                dash.isDashing = true;
                dash.remaining = dash.duration;
                rb.active = false;
                if (dash.direction.x == 0 && dash.direction.y == 0)
                    dash.direction.y = -1;
            }
            break;
        }
    }

    return hasRealInputs;
}
}
