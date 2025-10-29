/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Network Handler - Manages network events
*/

#include "Game.hpp"
#include <iostream>

void Game::processNetworkEvents()
{
    const auto event = m_events.pop();
    if (!event) {
        return;
    }

    const auto value = event.value();
    handleNetworkEvent(value);
}

void Game::handleNetworkEvent(const Client::NetworkEvent& event)
{
    switch (event.type) {
    case MessageType::CONNECT_ACK:
        handleConnectAck();
        break;
    case MessageType::LOBBY_INFO:
        handleLobbyInfo(event);
        break;
    case MessageType::SPAWN_ENTITY:
        handleSpawnEntity(event);
        break;
    case MessageType::DESPAWN_ENTITY:
        handleDespawnEntity(event);
        break;
    case MessageType::GAME_STATE:
        handleGameState(event);
        break;
    case MessageType::GAME_END_WIN:
        handleGameEndWin();
        break;
    case MessageType::GAME_END_LOSE:
        handleGameEndLose();
        break;
    case MessageType::USERNAME_ACK:
        handleUsername(event);
        break;
    case MessageType::KICK_NOTICE:
        handleKickPlayerNotice(event);
        break;
    case MessageType::AUTH_RESPONSE:
        handleAuthResponse(event);
        break;
    default:
        break;
    }
}

void Game::handleConnectAck()
{
    std::cout << "Connection acknowledged by server" << std::endl;
    m_connected = true;
    m_isConnecting = false;
    m_connectionTimeout = 0.0f;
    // CORRIGE : Appelle la méthode helper au lieu de naviguer directement
    onConnectionSuccess();
}

void Game::handleSpawnEntity(const Client::NetworkEvent& event)
{
    if (std::holds_alternative<Message>(event.payload)) {
        const Message& msg = std::get<Message>(event.payload);
        deserializeAndCreateEntity(msg, _registry);
    }
}

void Game::handleDespawnEntity(const Client::NetworkEvent& event)
{
    if (std::holds_alternative<Message>(event.payload)) {
        const Message& raw = std::get<Message>(event.payload);
        Message msg = raw;
        msg.resetReadPosition();
        uint32_t entityId = msg.readU32();

        Entity entity = findEntityByServerId(_registry, entityId);
        if (entity.id != 0) {
            _registry.kill_entity(entity);
        }
    }
}

void Game::handleGameState(const Client::NetworkEvent& event)
{
    if (std::holds_alternative<Message>(event.payload)) {
        const Message& msg = std::get<Message>(event.payload);
        deserializeAndUpdateGameState(msg, _registry);
    }
}

void Game::handleLobbyInfo(const Client::NetworkEvent& event)
{
    if (!std::holds_alternative<Message>(event.payload)) {
        return;
    }
    const Message& raw = std::get<Message>(event.payload);
    Message msg = raw;
    msg.resetReadPosition();
    uint32_t lobbyId = msg.readU32();

    if (lobbyId == 0) {
        std::cout << "Lobby not found or join failed" << std::endl;
        // Retour à la page de join en cas d'échec
        if (m_menu.isActive()) {
            m_menu.showJoinPage(_registry);
        }
        return;
    }
    uint8_t lobbyState = msg.readU8();
    const uint32_t ownerId = msg.readU32();
    const uint8_t numPlayers = msg.readU8();

    (void)lobbyState;
    m_currentLobbyId = lobbyId;
    m_lobbyOwnerId = ownerId;
    if (numPlayers != 0) {
        m_lobbyPlayers.clear();
        m_lobbyPlayerScores.clear();
    }
    for (uint8_t i = 0; i < numPlayers; i++) {
        uint32_t playerId = msg.readU32();
        const uint8_t playerUsernameLen = msg.readU8();
        const std::string username = msg.readString(playerUsernameLen);
        uint32_t xp = msg.readU32();
        m_lobbyPlayers[playerId] = username;
        m_lobbyPlayerScores[playerId] = xp;
    }

    std::cout << "Lobby operation confirmed by server - Lobby ID: " << lobbyId << std::endl;

    m_menu.setCurrentLobbyId(lobbyId);
    m_menu.setLobbyPlayerNames(m_lobbyPlayers);
    m_menu.setLobbyPlayerScores(m_lobbyPlayerScores);

    if (m_menu.getCurrentPage() != Menu::Page::WIN && m_menu.getCurrentPage() != Menu::Page::LOSE) {
        onLobbyJoined(lobbyId);
    }
}

void Game::handleGameEndWin()
{
    std::cout << "Game ended - You won!" << std::endl;
    m_currentLevel++;
    if (m_currentLevel > m_maxLevel) {
        m_currentLevel = 1;
    }

    // Load next level data
    std::string levelPath = "shared/res/levels/level" + std::to_string(m_currentLevel) + ".json";
    m_currentLevelData.loadFromJson(levelPath);

    clearGameEntities();
    // Utilise la méthode de navigation correcte
    if (m_menu.isActive()) {
        m_menu.showWinPage(_registry, m_currentLevel, m_maxLevel);
    } else {
        // Réactive le menu pour afficher la page Win
        _state = GameState::MENU;
        m_menu.activate(_registry, Menu::Page::WIN, m_currentLevel, m_maxLevel);
    }

    if (m_clientNetwork) {
        m_clientNetwork->lobbyInfoRequest();
    }
}

void Game::handleGameEndLose()
{
    std::cout << "Game ended - You lost!" << std::endl;
    m_currentLevel = 1;

    // Reload level 1 data
    std::string levelPath = "shared/res/levels/level1.json";
    m_currentLevelData.loadFromJson(levelPath);

    clearGameEntities();
    // Utilise la méthode de navigation correcte
    if (m_menu.isActive()) {
        m_menu.showLosePage(_registry, m_currentLevel, m_maxLevel);
    } else {
        // Réactive le menu pour afficher la page Lose
        _state = GameState::MENU;
        m_menu.activate(_registry, Menu::Page::LOSE, m_currentLevel, m_maxLevel);
    }

    if (m_clientNetwork) {
        m_clientNetwork->lobbyInfoRequest();
    }
}

void Game::handleUsername(const Client::NetworkEvent& event)
{
    if (std::holds_alternative<bool>(event.payload)) {
        const auto state = std::get<bool>(event.payload);
        std::cout << "Username creation state: " << state << std::endl;
    }
}

void Game::handleAuthResponse(const Client::NetworkEvent& event)
{
    if (!std::holds_alternative<Message>(event.payload)) {
        return;
    }
    const Message& raw = std::get<Message>(event.payload);
    const auto state = raw.readU32();
    if (state) {
        std::cout << "Login successful!" << std::endl;
        onLoginSuccess();
    } else {
        const auto messageLen = raw.readU8();
        const auto errMsg = raw.readString(messageLen);
        std::cout << "Login failed! Reason: " << errMsg << std::endl;
    }
}

void Game::handleKickPlayerNotice(const Client::NetworkEvent& event)
{
    if (!std::holds_alternative<Message>(event.payload)) {
        return;
    }
    const Message& raw = std::get<Message>(event.payload);
    Message msg = raw;
    msg.resetReadPosition();
    const uint32_t lobbyId = msg.readU32();
    const uint32_t playerId = msg.readU32();
    // REMOVE PLAYER Entity and remove it from the vector in the m_clientNetwork
    std::cout << "Removing player " << playerId << " from " << lobbyId << std::endl;
    m_lobbyPlayers.erase(playerId);
}
