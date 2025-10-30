/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** BossAttackTriggerSystem - Triggers boss attack animation when a boss fires
*/

#pragma once

#include "../../../shared/src/ecs/Registry.hpp"

// Detect newly spawned projectiles and, if owned by a boss, set IsAttacking on the boss for one frame
void bossAttackTriggerSystem(Registry& registry);
