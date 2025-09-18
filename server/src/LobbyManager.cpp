#include "LobbyManager.hpp"
#include <algorithm>
#include <iostream>

Lobby::Lobby(uint32_t lobbyId, uint32_t creator)
    : id(lobbyId)
    , creatorId(creator)
    , state(LobbyState::WAITING)
    , maxPlayers(4)
{
    players.push_back(creator);
}

LobbyManager::LobbyManager()
    : _nextLobbyId(1)
{
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
    if (lobby->creatorId != playerId) {
        return false;
    }

    if (lobby->state != LobbyState::WAITING) {
        return false;
    }

    if (lobby->players.size() < 1) {
        return false;
    }

    lobby->state = LobbyState::RUNNING;
    std::cout << "Game started in lobby " << lobbyId << " by player " << playerId << std::endl;
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

        if (lobby->players.empty()) {
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
