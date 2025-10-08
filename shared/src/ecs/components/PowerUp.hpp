/*
** PowerUp.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/shared/src/ecs/components
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Oct 7 1:53:00 PM 2025 jojo
** Last update Wed Oct 7 2:01:20 PM 2025 jojo
*/

#pragma once

enum class PowerUpType {
    HEAL,
    DAMAGE_BOOST
};

struct PowerUp {
    bool is_power = false;
    PowerUpType type = PowerUpType::HEAL;
    float effect_duration = 0.0f;
    float remaining_time = 0.0f;
};
