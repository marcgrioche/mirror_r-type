#include "RTypeServer.hpp"
#include "UdpSocket.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#ifdef _WIN32
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
{
    _socket = std::make_unique<UdpSocket>(_port);
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

    std::cout << "Server started on port " << _port << std::endl;
    _running = true;

    while (_running) {
#ifdef _WIN32
        if (stop_requested) {
            requestStop();
            break;
        }
#endif

        if (_socket->pollForData(100)) { // TODO : smart timeout
            std::vector<uint8_t> data;
            std::string sender_ip;
            uint16_t sender_port;

            ssize_t received = _socket->receive(data, sender_ip, sender_port);
            if (received > 0) {
                Message msg = Message::deserialize(data);
                handleReceive(msg, sender_ip, sender_port);
            }
        }
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
        std::vector<uint8_t> data = msg.serialize();
        _socket->send(data, it->second.ip_address, it->second.port);
    }
}

void RTypeServer::broadcast(const Message& msg)
{
    std::vector<uint8_t> data = msg.serialize();
    for (const auto& client : _clients) {
        _socket->send(data, client.second.ip_address, client.second.port);
    }
}

void RTypeServer::handleReceive(const Message& msg, const std::string& sender_ip, uint16_t sender_port)
{
    ClientInfo clientInfo = { msg.player_id, sender_ip, sender_port };

    auto it = _handlers.find(msg.getType());
    if (it != _handlers.end()) {
        it->second(msg, clientInfo);
    } else {
        std::cerr << "Unknown message type received: " << static_cast<int>(msg.getType()) << std::endl;
    }
}

void RTypeServer::registerHandlers()
{
    _handlers[MessageType::CONNECT] = [this](const Message& msg, ClientInfo& clientInfo) { handleConnect(msg, clientInfo); };
    _handlers[MessageType::INPUT] = [this](const Message& msg, ClientInfo& clientInfo) { handleInput(msg, clientInfo); };
    _handlers[MessageType::PING] = [this](const Message& msg, ClientInfo& clientInfo) { handlePing(msg, clientInfo); };
    _handlers[MessageType::DISCONNECT] = [this](const Message& msg, ClientInfo& clientInfo) { handleDisconnect(msg, clientInfo); };
    // Add more handlers as needed for other message types
}
