#include "LobbyManager.hpp"
#include "../../shared/include/Message.hpp"
#include "RTypeServer.hpp"
#include <algorithm>
#include <iostream>

Lobby::Lobby(uint32_t lobbyId, uint32_t creator)
    : id(lobbyId)
    , creatorId(creator)
    , state(LobbyState::WAITING)
    , maxPlayers(4)
    , gameInstance(nullptr)
    , threadRunning(false)
{
    players.push_back(creator);
}

Lobby::~Lobby()
{
    threadRunning = false;
    if (gameThread.joinable()) {
        gameThread.join();
    }
}

void Lobby::queueInput(const PlayerInput& input)
{
    std::lock_guard<std::mutex> lock(inputMutex);
    inputQueue.push(input);
}

bool Lobby::hasPendingInputs() const
{
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(inputMutex));
    return !inputQueue.empty();
}

PlayerInput Lobby::dequeueInput()
{
    std::lock_guard<std::mutex> lock(inputMutex);
    if (inputQueue.empty()) {
        return { 0, 0, {} };
    }
    PlayerInput input = inputQueue.front();
    inputQueue.pop();
    return input;
}

LobbyManager::LobbyManager()
    : _nextLobbyId(1)
    , _server(nullptr)
{
}

void LobbyManager::setServer(RTypeServer* server)
{
    _server = server;
}

uint32_t LobbyManager::createLobby(uint32_t creatorId)
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_playerToLobby.find(creatorId) != _playerToLobby.end()) {
        return 0;
    }

    uint32_t lobbyId = generateLobbyId();
    auto lobby = std::make_unique<Lobby>(lobbyId, creatorId);

    _lobbies[lobbyId] = std::move(lobby);
    _playerToLobby[creatorId] = lobbyId;

    std::cout << "Created lobby " << lobbyId << " for player " << creatorId << std::endl;
    return lobbyId;
}

bool LobbyManager::joinLobby(uint32_t lobbyId, uint32_t playerId)
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_playerToLobby.find(playerId) != _playerToLobby.end()) {
        return false;
    }

    auto it = _lobbies.find(lobbyId);
    if (it == _lobbies.end()) {
        return false;
    }

    Lobby* lobby = it->second.get();
    if (lobby->state != LobbyState::WAITING) {
        return false;
    }

    if (lobby->players.size() >= lobby->maxPlayers) {
        return false;
    }

    if (std::find(lobby->players.begin(), lobby->players.end(), playerId) != lobby->players.end()) {
        return false;
    }

    lobby->players.push_back(playerId);
    _playerToLobby[playerId] = lobbyId;

    std::cout << "Player " << playerId << " joined lobby " << lobbyId << std::endl;
    return true;
}

bool LobbyManager::startGame(uint32_t lobbyId, uint32_t playerId)
{
    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _lobbies.find(lobbyId);
    if (it == _lobbies.end()) {
        return false;
    }

    Lobby* lobby = it->second.get();
    if (lobby->state != LobbyState::WAITING) {
        return false;
    }

    if (lobby->players.size() < 1) {
        return false;
    }

    if (std::find(lobby->players.begin(), lobby->players.end(), playerId) == lobby->players.end()) {
        return false;
    }

    lobby->readyPlayers.insert(playerId);
    std::cout << "Player " << playerId << " is ready in lobby " << lobbyId << " (" << lobby->readyPlayers.size() << "/" << lobby->players.size() << " ready)" << std::endl;

    if (lobby->readyPlayers.size() != lobby->players.size()) {
        return false;
    }

    lobby->gameInstance = std::make_unique<GameInstance>(lobbyId);
    lobby->gameInstance->initialize();

    for (uint32_t player : lobby->players) {
        lobby->gameInstance->addPlayer(player);
    }

    if (lobby->gameThread.joinable()) {
        lobby->gameThread.join();
    }

    lobby->threadRunning = true;
    lobby->gameThread = std::thread(&LobbyManager::runLobbyThread, this, lobby);

    lobby->state = LobbyState::RUNNING;
    std::cout << "Game started in lobby " << lobbyId << " - all " << lobby->players.size() << " players ready" << std::endl;
    return true;
}

const Lobby* LobbyManager::getLobby(uint32_t lobbyId) const
{
    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _lobbies.find(lobbyId);
    if (it == _lobbies.end()) {
        return nullptr;
    }

    return it->second.get();
}

uint32_t LobbyManager::getPlayerLobby(uint32_t playerId) const
{
    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _playerToLobby.find(playerId);
    if (it == _playerToLobby.end()) {
        return 0;
    }

    return it->second;
}

bool LobbyManager::removePlayer(uint32_t playerId)
{
    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _playerToLobby.find(playerId);
    if (it == _playerToLobby.end()) {
        return false;
    }

    uint32_t lobbyId = it->second;
    _playerToLobby.erase(it);

    auto lobbyIt = _lobbies.find(lobbyId);
    if (lobbyIt != _lobbies.end()) {
        Lobby* lobby = lobbyIt->second.get();
        auto playerIt = std::find(lobby->players.begin(), lobby->players.end(), playerId);
        if (playerIt != lobby->players.end()) {
            lobby->players.erase(playerIt);
        }

        lobby->readyPlayers.erase(playerId);

        if (lobby->players.empty()) {
            lobby->threadRunning = false;
            if (lobby->gameThread.joinable()) {
                lobby->gameThread.join();
            }
            _lobbies.erase(lobbyIt);
            std::cout << "Removed empty lobby " << lobbyId << std::endl;
        } else if (lobby->creatorId == playerId) {
            lobby->creatorId = lobby->players[0];
            std::cout << "Transferred ownership of lobby " << lobbyId << " to player " << lobby->creatorId << std::endl;
        }
    }

    std::cout << "Removed player " << playerId << " from lobby " << lobbyId << std::endl;
    return true;
}

std::vector<uint32_t> LobbyManager::getActiveLobbies() const
{
    std::lock_guard<std::mutex> lock(_mutex);

    std::vector<uint32_t> activeLobbies;
    for (const auto& pair : _lobbies) {
        activeLobbies.push_back(pair.first);
    }

    return activeLobbies;
}

uint32_t LobbyManager::generateLobbyId()
{
    return _nextLobbyId++;
}

GameInstance* LobbyManager::getGameInstance(uint32_t lobbyId)
{
    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _lobbies.find(lobbyId);
    if (it == _lobbies.end()) {
        return nullptr;
    }

    Lobby* lobby = it->second.get();
    if (lobby->state != LobbyState::RUNNING || !lobby->gameInstance) {
        return nullptr;
    }

    return lobby->gameInstance.get();
}

std::vector<uint32_t> LobbyManager::getLobbyPlayers(uint32_t lobbyId) const
{
    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _lobbies.find(lobbyId);
    if (it == _lobbies.end()) {
        return {};
    }

    return it->second->players;
}

void LobbyManager::runLobbyThread(Lobby* lobby)
{
    // TODO: Refactor
    std::cout << "Started game thread for lobby " << lobby->id << std::endl;

    bool hadRealInputsThisTick = false;

    while (lobby->threadRunning) {
        hadRealInputsThisTick = false;

        while (lobby->hasPendingInputs()) {
            PlayerInput input = lobby->dequeueInput();
            bool hadRealInput = lobby->gameInstance->processPlayerInput(input.playerId, input.tick, input.inputs);
            if (hadRealInput) {
                hadRealInputsThisTick = true;
            }
        }

        lobby->gameInstance->update();

        if (lobby->gameInstance->hasWon()) {
            std::cout << "Game won in lobby " << lobby->id << std::endl;
            Message endMsg(MessageType::GAME_END_WIN);
            _server->broadcastToLobby(lobby->id, endMsg);
            lobby->state = LobbyState::WAITING;
            lobby->gameInstance.reset();
            lobby->readyPlayers.clear();
            lobby->threadRunning = false;
            break;
        } else if (lobby->gameInstance->hasLost()) {
            std::cout << "Game lost in lobby " << lobby->id << std::endl;
            Message endMsg(MessageType::GAME_END_LOSE);
            _server->broadcastToLobby(lobby->id, endMsg);
            lobby->state = LobbyState::WAITING;
            lobby->gameInstance.reset();
            lobby->readyPlayers.clear();
            lobby->threadRunning = false;
            break;
        }

        if (_server) {
            auto newEntities = lobby->gameInstance->getAndClearNewEntities();

            const size_t BATCH_SIZE = 10;
            for (size_t i = 0; i < newEntities.size(); i += BATCH_SIZE) {
                size_t batchEnd = (std::min)(i + BATCH_SIZE, newEntities.size());
                std::vector<Entity> batch(newEntities.begin() + i, newEntities.begin() + batchEnd);

                Message spawnMsg = lobby->gameInstance->serializeEntityBatch(batch);
                if (!spawnMsg.getPayload().empty()) {
                    _server->broadcastToLobby(lobby->id, spawnMsg);
                }
            }

            auto killedEntities = lobby->gameInstance->getAndClearKilledEntities();
            for (uint32_t entityId : killedEntities) {
                Message despawnMsg(MessageType::DESPAWN_ENTITY);
                despawnMsg.write(entityId);
                _server->broadcastToLobby(lobby->id, despawnMsg);
            }

            if (hadRealInputsThisTick || lobby->gameInstance->hasStateChanged()) {
                std::vector<uint8_t> gameStateData = lobby->gameInstance->serializeGameState();
                Message gameStateMsg(MessageType::GAME_STATE, gameStateData);
                _server->broadcastToLobby(lobby->id, gameStateMsg);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // 30 ticks per second
    }

    std::cout << "Game thread ended for lobby " << lobby->id << std::endl;
}
