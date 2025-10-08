/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** BoundarySystem - Handles screen boundary checking
*/

#include "BoundarySystem.hpp"
#include "../../include/Config.hpp"
#include "components/Position.hpp"
#include "components/Hitbox.hpp"
#include "components/Dead.hpp"
#include "components/Health.hpp"
#include "components/Tags.hpp"

void boundarySystem(Registry& registry)
{
    auto view = registry.view<Position, Hitbox, Health, Dead>();

    for (auto&& [pos, hitbox, health, dead] : view) {
        float X = pos.x + hitbox.offset_x + hitbox.width;
        float Y = pos.y + hitbox.offset_y + hitbox.height;

        if (Y > SCREEN_HEIGHT + 200.0f || X < -200.0f || X > SCREEN_WIDTH + 200.0f) {
            health.hp = 0;
            dead.dead = true;
        }
    }
}
