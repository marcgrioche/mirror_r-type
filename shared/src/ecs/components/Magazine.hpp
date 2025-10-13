/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Magazine - Component for managing weapon ammunition
*/

#pragma once

/**
 * @brief Component that manages weapon ammunition capacity and current ammo count
 * Tracks the maximum ammunition capacity and current available ammunition for weapons
 */
struct Magazine {
    int MaxAmmo = 30; /**< Maximum ammunition capacity of the weapon */
    int currentAmmo = MaxAmmo; /**< Current available ammunition (initialized to max) */
};
