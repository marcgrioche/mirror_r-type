#include "RTypeServer.hpp"
#include "UdpSocket.hpp"
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

static RTypeServer* currentInstance = nullptr;

void RTypeServer::handleSignal(int)
{
    if (currentInstance) {
        currentInstance->requestStop();
        std::cout << "\nStopping server gracefully..." << std::endl;
    }
}

RTypeServer::RTypeServer(uint16_t port)
    : _port(port)
{
    _socket = std::make_unique<UdpSocket>(_port);
    registerHandlers();
}

void RTypeServer::start()
{
    currentInstance = this;
    std::signal(SIGINT, handleSignal);

    std::cout << "Server started on port " << _port << std::endl;
    _running = true;

    _socket->asyncReceive([this](const std::vector<uint8_t>& data) {
        Message msg = Message::deserialize(data);
        handleReceive(msg);
    });

    while (_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    currentInstance = nullptr;
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
    (void)playerId;
    (void)msg;
    // TODO: Serialize message and send via UdpSocket to client endpoint
}

void RTypeServer::broadcast(const Message& msg)
{
    (void)msg;
    // TODO: Serialize message and send to all clients
}

void RTypeServer::handleReceive(const Message& msg)
{
    auto it = _handlers.find(msg.getType());
    if (it != _handlers.end()) {
        it->second(msg);
    } else {
        std::cerr << "Unknown message type received: " << static_cast<int>(msg.getType()) << std::endl;
    }
}

void RTypeServer::handleConnect(const Message& msg)
{
    // Handle CONNECT message (example logic)
    std::cout << "Client " << msg.player_id << " connected." << std::endl;
    if (_clients.find(msg.player_id) == _clients.end()) {
        _clients[msg.player_id] = ClientInfo { msg.player_id };
    }
    // Send CONNECT_ACK (not implemented here)
}

void RTypeServer::registerHandlers()
{
    _handlers[MessageType::CONNECT] = [this](const Message& msg) { handleConnect(msg); };
    _handlers[MessageType::INPUT] = [this](const Message& msg) { handleInput(msg); };
    _handlers[MessageType::PING] = [this](const Message& msg) { handlePing(msg); };
    _handlers[MessageType::DISCONNECT] = [this](const Message& msg) { handleDisconnect(msg); };
    // Add more handlers as needed for other message types
}
