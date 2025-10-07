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

void Game::interpolateEntityPosition(std::shared_ptr<Registry> t_registry, Entity& t_entity, float const posX,
    const float posY, const uint32_t t_serverTick)
{
    Position& localPos = t_registry->get<Position>(t_entity);
    const float dx = std::abs(localPos.x - posX);
    const float dy = std::abs(localPos.y - posY);
    constexpr float epsilon = 2.f; // Tolerance for floating point error
    constexpr float snapThreshold = 50.f;
    constexpr float alpha = 0.1f;

    std::cout << "Client tick: " << m_clientNetwork->getCurrentTick() << " PosX: " << localPos.x << " PosY: " << localPos.y << std::endl;
    std::cout << "Server tick: " << t_serverTick << " PosX: " << posX << " PosY: " << posY << std::endl;
    if (dx > snapThreshold || dy > snapThreshold) {
        localPos.x = posX;
        localPos.y = posY;
        m_inputHistory.removeUpToTick(t_serverTick);

        // 4. Reapply unconfirmed inputs
        for (const InputFrame& frame : m_inputHistory.getHistory()) {
            if (frame.tick > t_serverTick) {
                simulateLocalPlayerInput(t_entity, frame.actions, t_registry);
            }
        }
    } else if (dx > epsilon || dy > epsilon) {
        // Smoothly interpolate
        localPos.x = localPos.x * (1.0f - alpha) + posX * alpha;
        localPos.y = localPos.y * (1.0f - alpha) + posY * alpha;
    }
}

void Game::simulateLocalPlayerInput(Entity& t_player, const std::vector<std::pair<GameInput, bool>>& t_inputs,
    std::shared_ptr<Registry> t_registry)
{
    PlayerActions::updateVelocity(t_inputs, t_registry, t_player);

    movementSystem(t_registry, TICK_DURATION, t_player);
    updateEntitySpritePosition(t_registry, t_player);
    gravitySystem(t_registry, TICK_DURATION, t_player);
    collisionSystem(t_registry, TICK_DURATION);
    // Add any other systems that should run per tick
}
