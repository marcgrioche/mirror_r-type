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
#include "../../../../shared/src/ecs/components/Velocity.hpp"

void spriteAnimationSystem(Registry& registry, float deltaTime)
{
    auto view1 = registry.view<BossTag>();

    for (auto it = view1.begin(); it != view1.end(); ++it) {
        Entity e = it.entity();
        
        if (!registry.has<Sprite>(e) || !registry.has<Health>(e) || !registry.has<MaxHealth>(e)) {
            std::cout << "[SpriteAnimationSystem ACCA] Boss entity missing required components!" << std::endl;
            continue;
        }
        
        Sprite& sprite = registry.get<Sprite>(e);
        MaxHealth& max_health = registry.get<MaxHealth>(e);
        Health& health = registry.get<Health>(e);
        
        if (!registry.has<IsAttacking>(e)) {
            std::cout << "[SpriteAnimationSystem] Boss entity missing IsAttacking component!" << std::endl;
            continue;
        }
        
        IsAttacking& is_attacking = registry.get<IsAttacking>(e);

        std::cout << "[CLIENT SpriteAnimationSystem] Boss isAttacking = " << is_attacking.attacking << std::endl;
        if (is_attacking.attacking > 0) {
            std::cout << "[CLIENT] IS ATTACKING - Changing sprite!" << std::endl;
            sprite.current_id = sprite.attack_id;
            sprite.current_frameY = 0;
            is_attacking.attacking = 0;
            continue;
        }
        sprite.current_id = sprite.texture_id;
        for (int i = 0; i <= sprite.nb_state; i++) {
            if (health.hp < max_health.hp / (sprite.nb_state + 1) * i) {
                sprite.current_frameY = sprite.nb_state - i + 1;
                break;
            }
        }
    }

    auto view2 = registry.view<Sprite>();

    for (auto it = view2.begin(); it != view2.end(); ++it) {
        Entity e = it.entity();
        Sprite& sprite = registry.get<Sprite>(e);

        if (sprite.total_frames <= 1) {
            continue;
        }
        // if (registry.has<PlayerTag>(e) && registry.has<Velocity>(e) && registry.get<Velocity>(e).v.x == 0.0)
        //     continue;

        sprite.elapsed_time += deltaTime;

        if (sprite.elapsed_time >= sprite.frame_duration) {
            sprite.elapsed_time = 0.0f;

            sprite.current_frameX = (sprite.current_frameX + 1) % sprite.total_frames;

            // std::cout << "[CLIENT] current frame X = " << sprite.current_frameX << " current frame Y = " << sprite.current_frameY << std::endl;

            sprite.srcRect.x = sprite.current_frameX * sprite.frame_width;
            sprite.srcRect.y = sprite.current_frameY * sprite.frame_height;
            sprite.srcRect.w = sprite.frame_width;
            sprite.srcRect.h = sprite.frame_height;
        }
    }
}
