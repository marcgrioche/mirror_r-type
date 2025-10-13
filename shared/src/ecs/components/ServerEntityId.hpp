/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** ServerEntityId - Stores the server's entity ID for synchronization
*/

#pragma once

#include <cstdint>

/**
 * @brief Component that stores the server's unique identifier for an entity
 * Used for client-server synchronization to map client entities to their server counterparts
 */
struct ServerEntityId {
    uint32_t id = 0; /**< The unique identifier assigned by the server for this entity */
};
