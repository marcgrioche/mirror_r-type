#include "CreatePlatform.hpp"

Entity factories::createOneWayPlatform(Registry& registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Hitbox>(platform, Hitbox { 120, 20, 0, 0 });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<BottomPassPlatform>(platform);
    return platform;
}

Entity factories::createPlatform(Registry& registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position { posx, posy });
    registry.emplace<Hitbox>(platform, Hitbox { 120, 20, 0, 0 });
    registry.emplace<PlatformTag>(platform);
    registry.emplace<NoPassPlatform>(platform);
    return platform;
}

Entity factories::createPlatform(Registry& registry, const Position& position, const Hitbox& hitbox)
{
    Entity platform = registry.create_entity();
    registry.add<Position>(platform, position);
    registry.add<Hitbox>(platform, hitbox);
    registry.add<PlatformTag>(platform, PlatformTag {});
    registry.add<NoPassPlatform>(platform, NoPassPlatform {});
    return platform;
}
