/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** BoundarySystem - Handles screen boundary checking
*/

#include "BoundarySystem.hpp"
#include "../../include/Config.hpp"
#include "components/AllComponents.hpp"

void boundarySystem(Registry& registry)
{
    auto view = registry.view<PlayerTag, Position, Hitbox, Health>();

    for (auto&& [player, pos, hitbox, health] : view) {
        float playerBottom = pos.y + hitbox.offset_y + hitbox.height;

        if (playerBottom > SCREEN_HEIGHT + 200.0f) {
            health.hp = 0;
        }
    }
}
