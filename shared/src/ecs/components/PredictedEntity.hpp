/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Component to mark entities as client-predicted
*/

#pragma once

#include <cstdint>

/**
 * Tag component to mark entities that are predicted on the client side.
 * These entities should use predicted positions for rendering.
 */
struct PredictedEntity {
    uint32_t playerId;

    PredictedEntity(uint32_t id = 0)
        : playerId(id)
    {
    }
};
