#include "../../include/game_instance/GameInstancePlayer.hpp"
#include "../../include/Config.hpp"
#include "../../include/game_instance/PlayerInputProcessor.hpp"
#include "../ecs/components/Dash.hpp"
#include "../ecs/components/Health.hpp"
#include "../ecs/components/Position.hpp"
#include "../ecs/components/RigidBody.hpp"
#include "../ecs/components/Velocity.hpp"
#include "../ecs/systems/FrequencyUtils.hpp"
#include "../ecs/systems/WeaponSystem.hpp"
#include "../entities/player/CreatePlayer.hpp"
#include "components/componentutils/VectorUtils.hpp"
#include <iostream>

void GameInstancePlayer::addPlayer(Registry& registry, uint32_t playerId, const std::string& username,
    std::vector<Entity>& newEntities)
{
    std::cout << "Adding player " << playerId << " to game instance with username: " << username << std::endl;
    Entity playerEntity = factories::createPlayer(registry, username);
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
                                            std::vector<Entity>& newEntities,
                                            float mouseX, float mouseY)
{
    auto it = _playerEntities.find(playerId);
    if (it == _playerEntities.end())
        return false;

    Entity playerEntity = it->second;

    // Pass to the refactored function including mouse coordinates
    return PlayerInputProcessor::processInput(
        registry,
        playerEntity,
        tick,
        inputs,
        newEntities,
        playerId,
        false,
        mouseX,
        mouseY);
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
