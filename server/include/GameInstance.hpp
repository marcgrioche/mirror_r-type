#pragma once

#include "../../shared/include/GameInputs.hpp"
#include "../../shared/src/ecs/Registry.hpp"
#include "../../shared/src/ecs/components/AllComponents.hpp"
#include "../../shared/src/ecs/systems/CollisionSystem.hpp"
#include "../../shared/src/ecs/systems/GravitySystem.hpp"
#include "../../shared/src/ecs/systems/MovementSystem.hpp"
#include "../../shared/src/ecs/systems/ProjectileSystem.hpp"
#include "../../shared/src/entities/platform/CreatePlatform.hpp"
#include "../../shared/src/entities/player/CreatePlayer.hpp"
#include <chrono>
#include <cstdint>
#include <unordered_map>
#include <vector>

class GameInstance {
public:
    static constexpr uint32_t TICKS_PER_SECOND = 60;
    static constexpr float TICK_DURATION = 1.0f / TICKS_PER_SECOND;

    GameInstance(uint32_t lobbyId);
    ~GameInstance() = default;

    void initialize();
    void update();
    bool isRunning() const { return _isRunning; }
    void stop() { _isRunning = false; }

    uint32_t getLobbyId() const { return _lobbyId; }
    uint32_t getCurrentTick() const { return _currentTick; }

    void addPlayer(uint32_t playerId);
    void removePlayer(uint32_t playerId);
    void processPlayerInput(uint32_t playerId, uint32_t tick, const std::vector<std::pair<GameInput, bool>>& inputs);

    std::vector<uint8_t> serializeGameState() const;
    void deserializeGameState(const std::vector<uint8_t>& data);

private:
    uint32_t _lobbyId;
    Registry _registry;
    bool _isRunning;
    uint32_t _currentTick;
    std::chrono::steady_clock::time_point _lastTickTime;

    std::unordered_map<uint32_t, Entity> _playerEntities;

    void updateTick();
    void initializeLevel();
    void processInputs();
    void simulatePhysics();
    void checkCollisions();
    void cleanupEntities();
};
