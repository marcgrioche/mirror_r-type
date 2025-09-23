/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity
*/

#include "Entity.hpp"

bool Entity::operator==(Entity const& o) const noexcept
{
    return id == o.id && version == o.version;
}

bool Entity::operator!=(Entity const& o) const noexcept
{
    return !(*this == o);
}

std::ostream& operator<<(std::ostream& os, Entity const& e)
{
    os << "Entity{" << e.id << ":" << e.version << "}";
    return os;
}
