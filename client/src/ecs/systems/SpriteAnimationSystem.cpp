/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteAnimationSystem
*/

#include "SpriteAnimationSystem.hpp"
#include "components/Sprite.hpp"
#include "../../../../shared/src/ecs/components/Tags.hpp"
#include "../../../../shared/src/ecs/components/MaxHealth.hpp"
#include "../../../../shared/src/ecs/components/Health.hpp"
#include "../../../../shared/src/ecs/components/IsAttacking.hpp"

void spriteAnimationSystem(Registry& registry, float deltaTime)
{
    auto view1 = registry.view<BossTag>();

    for (auto it = view1.begin(); it != view1.end(); ++it) {
        Entity e = it.entity();
        Sprite& sprite = registry.get<Sprite>(e);
        MaxHealth max_health = registry.get<MaxHealth>(e);
        Health health = registry.get<Health>(e);
        IsAttacking& is_attacking = registry.get<IsAttacking>(e);

        if (is_attacking.attacking == true) {
            sprite.texture_id = "heads_monster_attack.png";
            sprite.current_frameY = 0;
            is_attacking.attacking = false;
            continue;
        }

        for (int i = 0; i < sprite.total_frames; i++) {
            if (health.hp < max_health.hp / (sprite.nb_state + 1) * i)
                sprite.current_frameY = sprite.nb_state - i + 1;
        }
    }

    auto view2 = registry.view<Sprite>();

    for (auto it = view2.begin(); it != view2.end(); ++it) {
        Entity e = it.entity();
        Sprite& sprite = registry.get<Sprite>(e);

        if (sprite.total_frames <= 1) {
            continue;
        }

        sprite.elapsed_time += deltaTime;

        if (sprite.elapsed_time >= sprite.frame_duration) {
            sprite.elapsed_time = 0.0f;

            sprite.current_frameX = (sprite.current_frameX + 1) % sprite.total_frames;

            sprite.srcRect.x = sprite.current_frameX * sprite.frame_width;
            sprite.srcRect.y = sprite.current_frameY * sprite.frame_height;
            sprite.srcRect.w = sprite.frame_width;
            sprite.srcRect.h = sprite.frame_height;
        }
    }
}
