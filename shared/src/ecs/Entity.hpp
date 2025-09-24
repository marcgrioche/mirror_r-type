/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity
*/

#pragma once
#include <cstdint>
#include <iostream>

constexpr Entity INVALID_ENTITY{std::numeric_limits<uint32_t>::max(), 
                                std::numeric_limits<uint32_t>::max()};

struct Entity {
    uint32_t id;
    uint32_t version;

    bool operator==(Entity const& o) const noexcept;
    bool operator!=(Entity const& o) const noexcept;
};

std::ostream& operator<<(std::ostream& os, Entity const& e);
