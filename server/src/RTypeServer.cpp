#include "RTypeServer.hpp"
#include "UdpSocket.hpp"
#include <iostream>

RTypeServer::RTypeServer(uint16_t port)
    : port_(port)
{
    // TODO: Initialize UdpSocket here
}

void RTypeServer::start()
{
    // TODO: Start async receive loop using UdpSocket
    std::cout << "Server started on port " << port_ << std::endl;
}

void RTypeServer::stop()
{
    // TODO: Stop socket and clean up
    std::cout << "Server stopped." << std::endl;
}

void RTypeServer::sendToClient(uint32_t playerId, const Message& msg)
{
    // TODO: Serialize message and send via UdpSocket to client endpoint
}

void RTypeServer::broadcast(const Message& msg)
{
    // TODO: Serialize message and send to all clients
}

void RTypeServer::handleReceive(const Message& msg)
{
    // TODO: Handle incoming message (dispatch by type)
}

void RTypeServer::handleConnect(const Message& msg)
{
    // TODO: Handle CONNECT message
}
