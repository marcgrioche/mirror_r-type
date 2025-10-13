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

/**
 * @brief Enumeration of different power-up types available in the game
 */
enum class PowerUpType {
    HEAL, /**< Restores health to the player */
    DAMAGE_BOOST /**< Temporarily increases player damage output */
};

/**
 * @brief Component representing power-up entities and their effects
 * @param is_power True if this entity is currently a power-up (can be collected)
 * @param type The type of power-up effect this entity provides
 * @param effect_duration How long the power-up effect lasts in seconds
 * @param remaining_time Time remaining for the active power-up effect
 */
struct PowerUp {
    bool is_power = false;
    PowerUpType type = PowerUpType::HEAL;
    float effect_duration = 0.0f;
    float remaining_time = 0.0f;
};
