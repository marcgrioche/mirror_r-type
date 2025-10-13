/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** HitboxUtils - Utilities for hitbox collision detection
*/

#pragma once
#include "../../Entity.hpp"
#include "../../Registry.hpp"
#include "../Hitbox.hpp"
#include "../Position.hpp"

/**
 * @brief Performs Axis-Aligned Bounding Box (AABB) collision test in world coordinates
 * Tests if two rectangles overlap by comparing their positions and dimensions
 * @param pa Position of the first entity
 * @param ha Hitbox component of the first entity
 * @param pb Position of the second entity
 * @param hb Hitbox component of the second entity
 * @return true if the hitboxes overlap, false otherwise
 */
inline bool aabb_overlap_world(const Position& pa, const Hitbox& ha,
    const Position& pb, const Hitbox& hb)
{
    float axmin = pa.x + ha.offset_x;
    float aymin = pa.y + ha.offset_y;
    float axmax = axmin + ha.width;
    float aymax = aymin + ha.height;

    float bxmin = pb.x + hb.offset_x;
    float bymin = pb.y + hb.offset_y;
    float bxmax = bxmin + hb.width;
    float bymax = bymin + hb.height;

    // no overlap if one is fully left/right/top/bottom of the other
    // treat edge-touching as non-collision (use <=). Change to < to include touching.
    if (axmax <= bxmin)
        return false;
    if (bxmax <= axmin)
        return false;
    if (aymax <= bymin)
        return false;
    if (bymax <= aymin)
        return false;
    return true;
}

/**
 * @brief High-level collision check between two entities
 * Checks if two entities collide by verifying they have required components and performing AABB test
 * @param registry The ECS registry containing all entities and components
 * @param a First entity to check for collision
 * @param b Second entity to check for collision
 * @return true if entities collide and both have Position and Hitbox components, false otherwise
 */
inline bool entities_collide(Registry& registry, Entity a, Entity b)
{
    if (!registry.has<Position>(a) || !registry.has<Hitbox>(a))
        return false;
    if (!registry.has<Position>(b) || !registry.has<Hitbox>(b))
        return false;

    const Position& pa = registry.get<Position>(a);
    const Hitbox& ha = registry.get<Hitbox>(a);
    const Position& pb = registry.get<Position>(b);
    const Hitbox& hb = registry.get<Hitbox>(b);

    return aabb_overlap_world(pa, ha, pb, hb);
}
