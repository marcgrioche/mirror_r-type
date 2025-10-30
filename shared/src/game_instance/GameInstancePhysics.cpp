#include "../../include/game_instance/GameInstancePhysics.hpp"
#include "../ecs/components/Position.hpp"
#include "../ecs/components/PreviousPosition.hpp"
#include "../ecs/systems/BoundarySystem.hpp"
#include "../ecs/systems/ColisionPlayerPowerUpSystem.hpp"
#include "../ecs/systems/CollisionEnemyProjectileSystem.hpp"
#include "../ecs/systems/CollisionPlayerProjectileSystem.hpp"
#include "../ecs/systems/CollisionSystem.hpp"
#include "../ecs/systems/DashSystem.hpp"
#include "../ecs/systems/HitEffectTimerSystem.hpp"
#include "../ecs/systems/MovementSystem.hpp"
#include "../ecs/systems/PowerUpEffectSystem.hpp"
#include "../ecs/systems/PowerUpSystem.hpp"
#include "../ecs/systems/RigidBodySystem.hpp"

void GameInstancePhysics::updatePreviousPositions(Registry& registry)
{
    auto prevPosView = registry.view<Position, PreviousPosition>();
    for (auto&& [pos, prevPos] : prevPosView) {
        prevPos.v.x = pos.v.x;
        prevPos.v.y = pos.v.y;
    }
}

int GameInstancePhysics::updateSystems(Registry& registry, float tickDuration)
{
    dashSystem(registry, tickDuration);
    rigidBodySystem(registry, tickDuration);
    int platformsToAdd = movementSystem(registry, tickDuration);
    boundarySystem(registry);
    powerUpEffectSystem(registry, tickDuration);
    powerUpSystem(registry, tickDuration);
    hitEffectTimerSystem(registry, tickDuration);

    return platformsToAdd;
}

void GameInstancePhysics::checkCollisions(Registry& registry, float tickDuration)
{
    collisionSystem(registry, tickDuration);
    collisionPlayerProjectileSystem(registry, tickDuration);
    collisionEnemyProjectileSystem(registry, tickDuration);
    collisionPlayerPowerUpSystem(registry, tickDuration);
}
