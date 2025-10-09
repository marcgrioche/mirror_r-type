/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteAnimationSystem
*/

#include "SpriteAnimationSystem.hpp"
#include "components/Sprite.hpp"

void spriteAnimationSystem(Registry& registry, float deltaTime)
{
    auto view = registry.view<Sprite>();

    for (auto it = view.begin(); it != view.end(); ++it) {
        Entity e = it.entity();
        Sprite& sprite = registry.get<Sprite>(e);

        if (sprite.total_frames <= 1) {
            continue;
        }

        sprite.elapsed_time += deltaTime;

        if (sprite.elapsed_time >= sprite.frame_duration) {
            sprite.elapsed_time = 0.0f;

            sprite.current_frame = (sprite.current_frame + 1) % sprite.total_frames;

            sprite.srcRect.x = sprite.current_frame * sprite.frame_width;
            sprite.srcRect.y = 0;
            sprite.srcRect.w = sprite.frame_width;
            sprite.srcRect.h = sprite.frame_height;
        }
    }
}
