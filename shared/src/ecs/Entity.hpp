/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity - Core ECS entity identifier with versioning for recycling
*/

#pragma once
#include <cstdint>
#include <iostream>
#include <limits>

/**
 * @brief Represents a unique entity in the ECS with versioning for safe recycling
 * @param id Unique identifier for the entity
 * @param version Version counter to prevent access to recycled entities
 */
struct Entity {
    uint32_t id;
    uint32_t version;

    /**
     * @brief Equality comparison operator
     * @param o Other entity to compare with
     * @return True if entities have the same id and version
     */
    bool operator==(Entity const& o) const noexcept;

    /**
     * @brief Inequality comparison operator
     * @param o Other entity to compare with
     * @return True if entities are different
     */
    bool operator!=(Entity const& o) const noexcept;
};

constexpr Entity INVALID_ENTITY { std::numeric_limits<uint32_t>::max(),
    std::numeric_limits<uint32_t>::max() };

std::ostream& operator<<(std::ostream& os, Entity const& e);
