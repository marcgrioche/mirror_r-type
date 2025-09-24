/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Player
*/
#include "CreatePlayer.hpp"

namespace factories {
    void createPlayer(Registry& registry)
    {
        Entity player = registry.create_entity();
        registry.emplace<Position>(player, 400.0f, 300.0f);
        registry.emplace<Velocity>(player, 0.0f, 0.0f);
        registry.emplace<Health>(player, 100);
        registry.emplace<Hitbox>(player, 50.0f, 50.0f, 0.0f, 0.0f);
        registry.emplace<Weight>(player, 1.0f);
        registry.emplace<PlayerTag>(player);
        registry.emplace<Sprite>(player, "player", SDL_Rect{0,0,623,623}, SDL_Rect{30,30,50,50});
        // registry.emplace<Sprite>(player, 0, 50, 50);
    }
}
