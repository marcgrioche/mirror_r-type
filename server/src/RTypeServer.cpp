#include "RTypeServer.hpp"
#include "IpEncoding.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
static std::atomic<bool> stop_requested { false };

BOOL WINAPI consoleCtrlHandler(DWORD ctrlType)
{
    if (ctrlType == CTRL_C_EVENT) {
        stop_requested = true;
        std::cout << "\nStopping server gracefully..." << std::endl;
        return TRUE;
    }
    return FALSE;
}
#else
#include <csignal>
static RTypeServer* currentInstance = nullptr;

void RTypeServer::handleSignal(int)
{
    if (currentInstance) {
        currentInstance->requestStop();
        std::cout << "\nStopping server gracefully..." << std::endl;
    }
}
#endif

RTypeServer::RTypeServer(uint16_t port)
    : _port(port)
    , _nextPlayerId(1)
{
    _socket = std::make_unique<UdpSocket>(_port);
    _lobbyManager.setServer(this);
    registerHandlers();
}

void RTypeServer::start()
{
#ifdef _WIN32
    SetConsoleCtrlHandler(consoleCtrlHandler, TRUE);
#else
    currentInstance = this;
    std::signal(SIGINT, handleSignal);
#endif

    std::string local_ip = getLocalIp();
    std::string encoded = encodeIp(local_ip, _port);
    std::cout << "Server started on " << encoded << std::endl;
    _running = true;

    while (_running) {
#ifdef _WIN32
        if (stop_requested) {
            requestStop();
            break;
        }
#endif

        processIncomingMessages();
        processOutgoingMessages();
    }

#ifndef _WIN32
    currentInstance = nullptr;
#endif
}

void RTypeServer::requestStop()
{
    _running = false;
}

void RTypeServer::stop()
{
    // TODO: Stop socket and clean up
    std::cout << "Server stopped." << std::endl;
}

void RTypeServer::sendToClient(uint32_t playerId, const Message& msg)
{
    auto it = _clients.find(playerId);
    if (it != _clients.end()) {
        queueMessage(msg, it->second);
    }
}

void RTypeServer::broadcast(const Message& msg)
{
    for (const auto& client : _clients) {
        queueMessage(msg, client.second);
    }
}

void RTypeServer::broadcastToLobby(uint32_t lobbyId, const Message& msg)
{
    std::vector<uint32_t> players = _lobbyManager.getLobbyPlayers(lobbyId);

    for (uint32_t playerId : players) {
        auto it = _clients.find(playerId);
        if (it != _clients.end()) {
            queueMessage(msg, it->second);
        }
    }
}

void RTypeServer::registerHandlers()
{
    _handlers[MessageType::CONNECT] = [this](const Message& msg, PeerInfo& peerInfo) { handleConnect(msg, peerInfo); };
    _handlers[MessageType::INPUT] = [this](const Message& msg, PeerInfo& peerInfo) { handleInput(msg, peerInfo); };
    _handlers[MessageType::PING] = [this](const Message& msg, PeerInfo& peerInfo) { handlePing(msg, peerInfo); };
    _handlers[MessageType::DISCONNECT] = [this](const Message& msg, PeerInfo& peerInfo) { handleDisconnect(msg, peerInfo); };
    _handlers[MessageType::CREATE_LOBBY] = [this](const Message& msg, PeerInfo& peerInfo) { handleCreateLobby(msg, peerInfo); };
    _handlers[MessageType::JOIN_LOBBY] = [this](const Message& msg, PeerInfo& peerInfo) { handleJoinLobby(msg, peerInfo); };
    _handlers[MessageType::START_GAME] = [this](const Message& msg, PeerInfo& peerInfo) { handleStartGame(msg, peerInfo); };
    _handlers[MessageType::LOBBY_STATE] = [this](const Message& msg, PeerInfo& peerInfo) { handleLobbyState(msg, peerInfo); };
    // Add more handlers as needed for other message types
}
