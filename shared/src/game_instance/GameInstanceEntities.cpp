#include "../../include/game_instance/GameInstanceEntities.hpp"
#include "../../include/Config.hpp"
#include "../ecs/components/Lifetime.hpp"
#include "../ecs/components/Dead.hpp"
#include "../ecs/components/Position.hpp"
#include "../ecs/components/Velocity.hpp"
#include "../ecs/components/Hitbox.hpp"
#include "../ecs/components/Health.hpp"
#include "../ecs/components/Tags.hpp"
#include "../ecs/components/PowerUp.hpp"
#include "../entities/platform/CreatePlatform.hpp"
#include "../entities/boss/CreateBoss.hpp"
#include "../entities/powerUp/CreatePowerUp.hpp"
#include <unordered_set>
#include <algorithm>
#include <cstdlib>

void GameInstanceEntities::initializeLevel(Registry& registry)
{
    auto platformList = factories::generateRandomPlatforms(registry, 22);
    _newEntitiesThisTick.insert(_newEntitiesThisTick.end(), platformList.begin(), platformList.end());
    
    // Create Boss
    _newEntitiesThisTick.push_back(factories::createBoss(
        registry, 
        Position { SCREEN_WIDTH - BOSS_WIDTH, 0.0f }, 
        Health { BOSS_HEALTH }, 
        Hitbox { BOSS_WIDTH, BOSS_HEIGHT }, 
        Velocity { 0.0f, 0.0f }
    ));
}

void GameInstanceEntities::cleanupEntities(Registry& registry, float tickDuration)
{
    // 1) Handle lifetime expiration
    std::vector<Entity> killedThisTick;
    {
        auto view = registry.view<Lifetime>();
        for (auto it = view.begin(); it != view.end(); ++it) {
            auto [lifetime] = *it;
            Entity entity = it.entity();
            lifetime.value -= tickDuration;
            if (lifetime.value <= 0.0f) {
                killedThisTick.push_back(entity);
            }
        }
    }

    // 2) Collect entities flagged Dead
    {
        auto viewDead = registry.view<Dead>();
        for (auto it = viewDead.begin(); it != viewDead.end(); ++it) {
            auto [dead] = *it;
            if (dead.dead == true) {
                killedThisTick.push_back(it.entity());
            }
        }
    }

    if (killedThisTick.empty())
        return;

    // 3) Record killed entity IDs (deduplicated) then physically remove from registry
    {
        std::unordered_set<uint32_t> already;
        for (auto e : killedThisTick) {
            if (already.insert(e.id).second) {
                _killedEntitiesThisTick.push_back(e.id);
            }
            
            // Drop power-ups from enemies
            if (registry.has<EnemyTag>(e) && registry.has<Position>(e)) {
                Position& pos = registry.get<Position>(e);
                PowerUpType type = (rand() % 2 == 0) ? PowerUpType::HEAL : PowerUpType::DAMAGE_BOOST;
                float effectDuration = (type == PowerUpType::DAMAGE_BOOST) ? 10.0f : 0.0f;
                _newEntitiesThisTick.push_back(factories::createPowerUp(
                    registry,
                    Position { pos.x, pos.y },
                    Velocity { -10.0f, 0.0f },
                    Hitbox { POWERUP_WIDTH, POWERUP_HEIGHT },
                    Lifetime { POWERUP_LIFETIME },
                    type,
                    effectDuration
                ));
            }
            registry.kill_entity(e);
        }
    }

    // 4) Remove from newly spawned list if they died the same tick
    if (!_newEntitiesThisTick.empty()) {
        _newEntitiesThisTick.erase(std::remove_if(_newEntitiesThisTick.begin(), _newEntitiesThisTick.end(), 
            [&](Entity const& cand) {
                for (auto const& k : killedThisTick) {
                    if (cand == k)
                        return true;
                }
                return false;
            }
        ), _newEntitiesThisTick.end());
    }
}

std::vector<Entity> GameInstanceEntities::getAndClearNewEntities()
{
    std::vector<Entity> entities = std::move(_newEntitiesThisTick);
    _newEntitiesThisTick.clear();
    return entities;
}

std::vector<uint32_t> GameInstanceEntities::getAndClearKilledEntities()
{
    std::vector<uint32_t> entities = std::move(_killedEntitiesThisTick);
    _killedEntitiesThisTick.clear();
    return entities;
}

void GameInstanceEntities::spawnRandomPowerUps(Registry& registry, int count)
{
    for (int i = 0; i < count; ++i) {
        float x = static_cast<float>(rand() % SCREEN_WIDTH + 100);
        float y = static_cast<float>(rand() % 300 + 200);
        
        PowerUpType type = (rand() % 2 == 0) ? PowerUpType::HEAL : PowerUpType::DAMAGE_BOOST;
        float effectDuration = (type == PowerUpType::DAMAGE_BOOST) ? 10.0f : 0.0f;

        Entity powerUp = factories::createPowerUp(
            registry,
            Position { x, y },
            Velocity { 0.0f, 0.0f },
            Hitbox { 20.0f, 20.0f, 0.0f, 0.0f },
            Lifetime { 30.0f },
            type,
            effectDuration
        );
        _newEntitiesThisTick.push_back(powerUp);
    }
}

void GameInstanceEntities::generatePlatforms(Registry& registry, int count)
{
    for (int i = 0; i < count; ++i) {
        auto platformTmp = factories::reGenerateRandomPlatforms(registry, 1);
        _newEntitiesThisTick.insert(_newEntitiesThisTick.end(), platformTmp.begin(), platformTmp.end());
    }
}
