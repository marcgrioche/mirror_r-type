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
    : port_(port)
{
    // TODO: Initialize UdpSocket here
}

void RTypeServer::start()
{
    currentInstance = this;
    std::signal(SIGINT, handleSignal);

    std::cout << "Server started on port " << port_ << std::endl;
    _running = true;
    while (_running) {
        // TODO: Add server tick, event handling, etc.
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
    (void)msg;
    // TODO: Handle incoming message (dispatch by type)
}

void RTypeServer::handleConnect(const Message& msg)
{
    (void)msg;
    // TODO: Handle CONNECT message
}
