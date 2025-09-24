#include "CreatePlatform.hpp"
#include "Platform.hpp"
#include "Position.hpp"
#include "Hitbox.hpp"
#include "Tags.hpp"


Entity factories::createOneWayPlatform(Registry& registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position{posx, posy});
    registry.emplace<Hitbox>(platform, Hitbox{120, 20, 0, 0});
    registry.emplace<PlatformTag>(platform);
    registry.emplace<BottomPassPlatform>(platform);
    return platform;
}

Entity factories::createPlatform(Registry &registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position{posx, posy});
    registry.emplace<Hitbox>(platform, Hitbox{120, 20, 0, 0});
    registry.emplace<PlatformTag>(platform);
    registry.emplace<NoPassPlatform>(platform);
    return platform;
}
