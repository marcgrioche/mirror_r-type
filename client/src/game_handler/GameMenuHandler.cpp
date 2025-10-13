/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Game Menu Handler - Manages menu events and transitions
*/

#include "Game.hpp"
#include "IpEncoding.hpp"
#include <iostream>

void Game::handleMenuConnectRequest()
{
    std::string connectionCode = m_menu.getConnectionCode(_registry);

    std::string ip;
    int port;
    decodeIp(connectionCode, ip, port);
    ip = ip.empty() ? "127.0.0.1" : ip;
    port = (port == 0) ? 4242 : port;

    m_clientNetwork = std::make_unique<Client::RTypeClient>(ip, port, m_clientPort, m_events);
    m_networkThread = std::thread([this]() { m_clientNetwork->start(); });
    m_clientNetwork->connectToServerRequest();

    m_menu.showHomePage(_registry);
    std::cout << "Connected to server " << ip << ":" << port << std::endl;
}

void Game::handleMenuCreateLobbyRequest()
{
    if (!m_clientNetwork) {
        std::cout << "ERROR: Not connected to server. Use Connect first." << std::endl;
        m_menu.showConnectionPage(_registry);
        return;
    }

    std::string pseudo = m_menu.getUserPseudo(_registry);

    m_clientNetwork->createLobbyRequest();
    printf("Lobby creation requested by %s, waiting for server response...\n", pseudo.c_str());
}

void Game::handleMenuJoinLobbyRequest()
{
    if (!m_clientNetwork) {
        std::cout << "ERROR: Not connected to server. Use Connect first." << std::endl;
        m_menu.showConnectionPage(_registry);
        return;
    }

    std::string lobbyCode = m_menu.getJoinCode(_registry);
    std::string pseudo = m_menu.getUserPseudo(_registry);

    if (lobbyCode.empty()) {
        std::cout << "ERROR: Please enter a lobby ID" << std::endl;
        return;
    }

    try {
        uint32_t lobbyId = static_cast<uint32_t>(std::stoul(lobbyCode));
        m_clientNetwork->joinLobbyRequest(lobbyId);
        std::cout << "Join lobby " << lobbyId << " requested by " << pseudo << ", waiting for server response..." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ERROR: Invalid lobby ID: " << lobbyCode << " - " << e.what() << std::endl;
        return;
    }
}

void Game::handleMenuStartRequest()
{
    if (!m_clientNetwork) {
        std::cout << "ERROR: Not connected to server." << std::endl;
        return;
    }

    std::cout << "Starting game..." << std::endl;
    m_clientNetwork->startGameRequest();
    m_menu.deactivate(_registry);
    startGameplay();
}

void Game::processMenuEvents()
{
    // Vérifie les demandes de connexion
    if (m_menu.hasConnectionRequest()) {
        handleMenuConnectRequest();
        m_menu.clearAllRequests();
        return;
    }

    // Vérifie les demandes de création de lobby
    if (m_menu.hasCreateRequest()) {
        handleMenuCreateLobbyRequest();
        m_menu.clearAllRequests();
        return;
    }

    // Vérifie les demandes de rejoindre un lobby
    if (m_menu.hasJoinRequest()) {
        handleMenuJoinLobbyRequest();
        m_menu.clearAllRequests();
        return;
    }

    // NOUVEAU : Vérifie les demandes de start depuis LobbyMenu
    if (m_menu.hasLobbyRequest()) {
        handleMenuStartRequest();
        m_menu.clearAllRequests();
        return;
    }
}

void Game::onConnectionSuccess()
{
    m_menu.showHomePage(_registry);
}

void Game::onLobbyJoined(uint32_t lobbyId)
{
    (void)lobbyId;
    m_menu.showLobbyPage(_registry);
}

void Game::onLobbyCreated(uint32_t lobbyId)
{
    (void)lobbyId;
    m_menu.showLobbyPage(_registry);
}

void Game::onGameStarted()
{
    m_menu.deactivate(_registry);
    startGameplay();
}
