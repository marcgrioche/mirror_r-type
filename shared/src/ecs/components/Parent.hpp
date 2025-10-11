/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Parent Component - Hierarchical relationship tracking for entities
*/

#pragma once
#include "Entity.hpp"

/**
 * @brief Component representing parent-child relationship between entities
 * @param parent The parent entity in the hierarchy
 */
struct Parent {
    Entity parent;
};
