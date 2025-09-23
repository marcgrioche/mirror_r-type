#pragma once
#include "Components.hpp"
#include "../Entity.hpp"
#include "../Registry.hpp"

// AABB test in world coordinates (Position + Hitbox.offset)
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

// High-level: check two entities (must have Position + Hitbox)
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
