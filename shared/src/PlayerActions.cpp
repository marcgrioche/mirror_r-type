#include "PlayerActions.hpp"

bool PlayerActions::updateVelocity(const std::vector<std::pair<GameInput, bool>>& t_inputs,
    Registry& t_registry, Entity& t_player)
{
    if (!t_registry.has<Velocity>(t_player) || !t_registry.has<Jump>(t_player)) {
        return false;
    }

    auto& velocity = t_registry.get<Velocity>(t_player);
    auto& jump = t_registry.get<Jump>(t_player);

    velocity.dx = 0.0f;

    const float speed = 250.0f;
    bool hasRealInputs = false;

    for (const auto& [input, isPressed] : t_inputs) {
        if (!isPressed)
            continue;

        hasRealInputs = true;
        switch (input) {
        case GameInput::UP:
            if (!jump.isJumping && jump.canJump) {
                velocity.dy = -V0;
                jump.isJumping = true;
                jump.canJump = false;
            }
            break;
        case GameInput::DOWN:
            if (jump.isJumping && velocity.dy > 0) {
                velocity.dy += 300.0f; // Fast-fall
            }
            break;
        case GameInput::LEFT:
            velocity.dx = -speed;
            break;
        case GameInput::RIGHT:
            velocity.dx = speed;
            break;
        case GameInput::ATTACK:
            // TODO: Handle shooting/projectile creation
            break;
        case GameInput::DASH:
            // TODO: Handle dash ability
            break;
        }
    }
    return hasRealInputs;
}
