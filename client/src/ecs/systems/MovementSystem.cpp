/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** MovementSystem implementation
*/

#include "MovementSystem.hpp"
#include "components/AllComponents.hpp"

void movementSystem(Registry& registry, float deltaTime)
{
    // store previous positions for entities that need it
    auto prevPosView = registry.view<Position, PreviousPosition>();
    for (auto&& [pos, prevPos] : prevPosView) {
        prevPos.x = pos.x;
        prevPos.y = pos.y;
    }
    
    auto view = registry.view<Position, Velocity>();
    for (auto&& [pos, vel] : view) {
        pos.x += vel.dx * deltaTime;
        pos.y += vel.dy * deltaTime;
        
        // garde le joueur dans l'écran, fait à l'arrache
        if (pos.x < 0) {
            pos.x = 0;
            vel.dx = 0;
        }
        if (pos.x > 800 - 32) { // Assuming player width of 32
            pos.x = 800 - 32;
            vel.dx = 0;
        }
    }
}
