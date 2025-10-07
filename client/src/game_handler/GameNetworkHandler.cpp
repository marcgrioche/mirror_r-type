/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Network Handler - Manages network events and local game updates
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
    case MessageType::GAME_STATE:
        handleGameState(event);
        break;
    case MessageType::GAME_END_WIN:
        handleGameEndWin();
        break;
    case MessageType::GAME_END_LOSE:
        handleGameEndLose();
        break;
    default:
        break;
    }
}

void Game::handleConnectAck()
{
    std::cout << "Connection acknowledged by server" << std::endl;
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

    std::cout << "Lobby operation confirmed by server - Lobby ID: " << lobbyId << std::endl;

    onLobbyJoined(lobbyId);
}

void Game::handleGameEndWin()
{
    std::cout << "Game ended - You won!" << std::endl;
    // Utilise la méthode de navigation correcte
    if (m_menu.isActive()) {
        m_menu.showWinPage(_registry);
    } else {
        // Réactive le menu pour afficher la page Win
        m_menu.activate(_registry, Menu::Page::WIN);
    }
}

void Game::handleGameEndLose()
{
    std::cout << "Game ended - You lost!" << std::endl;
    // Utilise la méthode de navigation correcte
    if (m_menu.isActive()) {
        m_menu.showLosePage(_registry);
    } else {
        // Réactive le menu pour afficher la page Lose
        m_menu.activate(_registry, Menu::Page::LOSE);
    }
}

void Game::processLocalGameUpdates()
{
    if (!m_localGameInstance) {
        return;
    }

    m_localGameInstance->update();
    processNewLocalEntities();
}

void Game::processNewLocalEntities()
{
    auto newEntities = m_localGameInstance->getAndClearNewEntities();

    for (const auto& entity : newEntities) {
        createLocalEntity(entity);
    }
}

void Game::createLocalEntity(Entity entity)
{
    Message msg = m_localGameInstance->serializeEntitySpawn(entity);
    if (msg.getPayload().empty()) {
        return;
    }
    deserializeAndCreateEntity(msg, _registry);
}
