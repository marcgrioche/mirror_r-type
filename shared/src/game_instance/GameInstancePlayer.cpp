#include "../../include/game_instance/GameInstancePlayer.hpp"
#include "../../include/Config.hpp"
#include "../ecs/components/Velocity.hpp"
#include "../ecs/components/Dash.hpp"
#include "../ecs/components/Health.hpp"
#include "../ecs/components/Position.hpp"
#include "../ecs/components/RigidBody.hpp"
#include "../ecs/systems/WeaponSystem.hpp"
#include "../ecs/systems/FrequencyUtils.hpp"
#include "../entities/player/CreatePlayer.hpp"
#include <iostream>
#include "components/componentutils/VectorUtils.hpp"

void GameInstancePlayer::addPlayer(Registry& registry, uint32_t playerId, const std::string& username,
                                   std::vector<Entity>& newEntities)
{
    std::cout << "Adding player " << playerId << " to game instance with username: " << username << std::endl;
    Entity playerEntity = factories::createPlayer(registry);
    _playerEntities[playerId] = playerEntity;
    newEntities.push_back(playerEntity);
    _usernames[playerEntity.id] = username;
}

void GameInstancePlayer::removePlayer(Registry& registry, uint32_t playerId)
{
    auto it = _playerEntities.find(playerId);
    if (it != _playerEntities.end()) {
        registry.kill_entity(it->second);
        _playerEntities.erase(it);
        std::cout << "Removed player " << playerId << " from game instance" << std::endl;
    }
}

bool GameInstancePlayer::processPlayerInput(Registry& registry, uint32_t playerId, uint32_t tick,
                                            const std::vector<std::pair<GameInput, bool>>& inputs,
                                            std::vector<Entity>& newEntities)
{
    (void)tick; // currently unused (reserved for rollback / prediction tick validation)
    
    auto it = _playerEntities.find(playerId);
    if (it == _playerEntities.end())
        return false;

    Entity playerEntity = it->second;

    if (!registry.has<Velocity>(playerEntity) || !registry.has<RigidBody>(playerEntity) || !registry.has<Dash>(playerEntity)) {
        return false;
    }

    auto& velocity = registry.get<Velocity>(playerEntity);
    auto& dash = registry.get<Dash>(playerEntity);
    auto& rb = registry.get<RigidBody>(playerEntity);

    if (!dash.isDashing) {
        velocity.v.x = 0.0f;
        dash.direction = { 0.0f, 0.0f };
    }

    const float speed = 250.0f;
    bool hasRealInputs = false;
    float accel = rb.isOnPlatform ? rb.groundAccel : rb.airAccel;

    for (const auto& [input, isPressed] : inputs) {
        if (!isPressed)
            continue;
        hasRealInputs = true;

        switch (input) {
        case GameInput::UP:
            if (!dash.isDashing) {
                if (rb.isOnPlatform) {
                    velocity.v.y = -V0;
                }
                dash.direction.y = -1;
            }
            break;
        case GameInput::DOWN:
            if (!dash.isDashing) {
                dash.direction.y = 1;
            }
            break;
        case GameInput::LEFT:
            if (!dash.isDashing) {
                //velocity.v.x = -speed; //useless ???
                rb.acceleration.x = speed * -accel;
                dash.direction.x = -1;
            }
            break;
        case GameInput::RIGHT:
            if (!dash.isDashing) {
                //velocity.v.x = speed;
                rb.acceleration.x = speed * accel;
                dash.direction.x = 1;
            }
            break;
        case GameInput::ATTACK:
            if (WeaponSystem::handlePlayerAttack(registry, playerEntity, playerId, newEntities)) {
                // Attack processed
            }
            break;
        case GameInput::DASH:
            if (!dash.isDashing && FrequencyUtils::shouldTrigger(dash.cooldown)) {
                dash.isDashing = true;
                dash.remaining = dash.duration;
                rb.active = false;
                if (dash.direction.x == 0 && dash.direction.y == 0)
                    dash.direction.y = -1;
            }
            break;
        }
    }

    return hasRealInputs;
}

uint32_t GameInstancePlayer::findPlayerIdByEntity(const Entity& entity) const
{
    for (const auto& [playerId, playerEntity] : _playerEntities) {
        if (playerEntity == entity) {
            return playerId;
        }
    }
    return 0; // Not found
}

std::optional<uint32_t> GameInstancePlayer::getPlayerEntityIdById(uint32_t playerId) const
{
    auto it = _playerEntities.find(playerId);
    if (it == _playerEntities.end()) {
        return std::nullopt;
    }
    return it->second.id;
}

bool GameInstancePlayer::cleanupDeadPlayers(Registry& registry, const std::vector<Entity>& killedEntities)
{
    bool anyRemoved = false;
    for (auto it = _playerEntities.begin(); it != _playerEntities.end();) {
        Entity e = it->second;
        bool wasKilled = false;
        for (auto const& k : killedEntities) {
            if (k == e) {
                wasKilled = true;
                break;
            }
        }
        if (wasKilled || (!registry.has<Health>(e) && !registry.has<Position>(e))) {
            it = _playerEntities.erase(it);
            anyRemoved = true;
        } else {
            ++it;
        }
    }
    return anyRemoved;
}
