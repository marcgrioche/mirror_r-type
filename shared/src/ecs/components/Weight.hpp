/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Weight Component - Defines entity mass for physics calculations
*/

#pragma once

/**
 * @brief Component that defines the mass/weight of an entity for physics calculations
 * Used by gravity and collision systems to determine how entities respond to physical forces
 */
struct Weight {
    float value; /**< The mass/weight value used in physics calculations */
};
