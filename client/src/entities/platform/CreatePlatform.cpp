#include "CreatePlatform.hpp"
#include "Platform.hpp"
#include "Position.hpp"
#include "Hitbox.hpp"
#include "Tags.hpp"


Entity factories::createOneWayPlatform(Registry& registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position{posx, posy});
    registry.emplace<Hitbox>(platform, Hitbox{100, 10, 0, 0});
    registry.emplace<PlatformTag>(platform);
    registry.emplace<OneWayPlatform>(platform);
    return Entity(); //not over, i start with basic platform
}

Entity factories::createPlatform(Registry &registry, float posx, float posy)
{
    Entity platform = registry.create_entity();
    registry.emplace<Position>(platform, Position{posx, posy});
    registry.emplace<Hitbox>(platform, Hitbox{100, 10, 0, 0});
    registry.emplace<PlatformTag>(platform);
    return Entity();
}
