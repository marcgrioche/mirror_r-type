/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Prediction - Client-side prediction and reconciliation (DISABLED)
*/

// Client-side prediction has been removed
// This file is kept empty for compatibility

#include "Game.hpp"
#include "PlayerActions.hpp"

void Game::interpolateEntityPosition(Registry& t_registry, Entity& t_entity, float const posX,
    const float posY, const uint32_t t_serverTick)
{
    Position& localPos = t_registry.get<Position>(t_entity);
    const float dx = std::abs(localPos.x - posX);
    const float dy = std::abs(localPos.y - posY);
    constexpr float epsilon = 0.5f; // Tolerance for floating point error

    if (dx > epsilon || dy > epsilon) {
        // 2. Snap to authoritative state
        localPos.x = posX;
        localPos.y = posY;

        // 3. Remove confirmed inputs
        m_inputHistory.removeUpToTick(t_serverTick);

        // 4. Reapply unconfirmed inputs
        for (const InputFrame& frame : m_inputHistory.getHistory()) {
            if (frame.tick > t_serverTick) {
                simulateLocalPlayerInput(t_entity, frame.actions, t_registry);
            }
        }
    }
}

void Game::simulateLocalPlayerInput(Entity& t_player, const std::vector<std::pair<GameInput, bool>>& t_inputs,
    Registry& t_registry)
{
    PlayerActions::updateVelocity(t_inputs, t_registry, t_player);

    // 2. Run movement and physics for just this player for one tick
    // If your movement system works on the whole registry, that's fine,
    // as long as only the local player is being corrected.
    movementSystem(t_registry, TICK_DURATION);
    updateEntitySpritePosition(t_registry, t_player);
    gravitySystem(t_registry, TICK_DURATION);
    projectileSystem(t_registry, TICK_DURATION);
    collisionSystem(t_registry, TICK_DURATION);
    // Add any other systems that should run per tick
}
