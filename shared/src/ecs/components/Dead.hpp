/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Dead Component - Death state flag for entities
*/

#pragma once

/**
 * @brief Component representing death state of an entity
 * @param dead True if the entity should be destroyed/removed from the game
 */
struct Dead {
    bool dead = false;
};
