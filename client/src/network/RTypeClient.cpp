//
// Created by dytomaomar on 9/17/25.
//

#include "RTypeClient.hpp"

#include <csignal>
#include <iostream>

using namespace Client;

static RTypeClient* instance;

RTypeClient::RTypeClient(const uint16_t port)
    : m_port(port)
{
    _socket = std::make_unique<UdpSocket>(m_port);
}

void RTypeClient::start()
{
    instance = this;
    std::signal(SIGINT, SIG_DFL);
}

void RTypeClient::stop()
{
    _running = false;
    std::cout << "Client closing connection" << std::endl;
    if (_socket != nullptr) {
        _socket->close();
    }
}

void RTypeClient::handleSignal(int)
{
    if (instance) {
        instance->requestStop();
        std::cout << "\nStopping server gracefully..." << std::endl;
    }
}
