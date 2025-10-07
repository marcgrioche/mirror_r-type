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
    case MessageType::GAME_RUNNING:
        handleGameRunning(event);
        break;
    default:
        break;
    }
}

void Game::handleConnectAck()
{
    std::cout << "Connection acknowledged by server" << std::endl;
}

void Game::handleSpawnEntity(const Client::NetworkEvent& event)
{
    if (std::holds_alternative<Message>(event.payload)) {
        const Message& msg = std::get<Message>(event.payload);
        deserializeAndCreateEntity(msg, m_localGameInstance->getRegistry());
    }
}

void Game::handleGameState(const Client::NetworkEvent& event)
{
    if (std::holds_alternative<Message>(event.payload)) {
        const Message& msg = std::get<Message>(event.payload);
        deserializeAndUpdateGameState(msg, m_localGameInstance->getRegistry());
    }
}

void Game::handleGameRunning(const Client::NetworkEvent& event)
{
    _accumulatedTime = 0.0;
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
        // Optionnel: revenir à Lobby page
        m_menu.activate(Menu::Page::Lobby);
        return;
    }

    std::cout << "Joined lobby " << lobbyId << " (server confirmed)" << std::endl;
    m_localGameInstance = std::make_unique<GameInstance>(lobbyId, _registry);
    m_menu.onCreated();
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
    deserializeAndCreateEntity(msg, m_localGameInstance->getRegistry());
}
