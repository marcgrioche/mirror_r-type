/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Health Component - Hit points and damage tracking for entities
*/

#pragma once

/**
 * @brief Component representing health/damage state of an entity
 * @param hp Current hit points (health value)
 */
struct Health {
    int hp;
    int hit = 0;
    float hitTime = 0.0f;
};
