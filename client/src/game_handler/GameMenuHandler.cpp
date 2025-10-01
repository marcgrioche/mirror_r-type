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
    std::string ip;
    int port;
    decodeIp(m_menu.m_inputCode, ip, port);
    ip = ip.empty() ? "127.0.0.1" : ip;
    port = (port == 0) ? 4242 : port;

    m_clientNetwork = std::make_unique<Client::RTypeClient>(ip, port, 2020, m_events);
    m_networkThread = std::thread([this]() { m_clientNetwork->start(); });
    m_clientNetwork->connectToServerRequest();
    m_menu.onConnected();
}

void Game::handleMenuCreateLobbyRequest()
{
    m_clientNetwork->createLobbyRequest();
    m_menu.onCreated();
    printf("Lobby created, waiting for game start...\n");
}

void Game::handleMenuJoinLobbyRequest()
{
    m_clientNetwork->joinLobbyRequest(std::stoi(m_menu.m_inputCode));
    m_menu.deactivate(_registry);
    startGameplay();
}

void Game::handleMenuStartRequest()
{
    m_clientNetwork->startGameRequest();
    m_menu.deactivate(_registry);
    startGameplay();
}

void Game::processMenuEvents()
{
    if (m_menu.popConnectRequest()) {
        handleMenuConnectRequest();
    }

    if (m_menu.popCreateLobbyRequest()) {
        handleMenuCreateLobbyRequest();
    }

    if (m_menu.popJoinLobbyRequest()) {
        handleMenuJoinLobbyRequest();
    }

    if (m_menu.popStartRequest()) {
        handleMenuStartRequest();
    }
}
