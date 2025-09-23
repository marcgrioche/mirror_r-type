//
// Created by dytomaomar on 9/17/25.
//

#include "RTypeClient.hpp"

#include <csignal>
#include <iostream>

using namespace Client;

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
static RTypeClient* instance;

void RTypeClient::handleSignal(int)
{
    if (instance) {
        instance->requestStop();
        std::cout << "\nStopping server gracefully..." << std::endl;
    }
}
#endif

void RTypeClient::requestStop()
{
    _running = false;
}

RTypeClient::RTypeClient(const std::string& t_serverIpAddress, const uint16_t t_serverPort,
    const uint16_t t_port, NetworkEventQueue& t_eventsQueue)
    : m_port(t_port)
    , m_serverInfo { 0, t_serverIpAddress, t_serverPort }
    , m_debug(false)
    , m_msgSequenceNumber(0)
    , m_ping(0)
    , m_eventsQueue(t_eventsQueue)
{
    _socket = std::make_unique<UdpSocket>(m_port);
}

RTypeClient::RTypeClient(const uint16_t t_port, NetworkEventQueue& t_eventsQueue)
    : m_port(t_port)
    , m_debug(false)
    , m_msgSequenceNumber(0)
    , m_ping(0)
    , m_eventsQueue(t_eventsQueue)
{
    m_serverInfo = { 0, "", 4242 };
}

// setters
void RTypeClient::setServerInfo(const std::string& t_serverIpAddress, const uint16_t t_serverPort)
{
    m_serverInfo = { 0, t_serverIpAddress, t_serverPort };
}

void RTypeClient::setDebugMode(const bool t_debugMode)
{
    m_debug = t_debugMode;
}

void RTypeClient::start()
{
#ifdef _WIN32
    SetConsoleCtrlHandler(consoleCtrlHandler, TRUE);
#else
    instance = this;
    std::signal(SIGINT, SIG_DFL);
#endif
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
    instance = nullptr;
#endif
}

void RTypeClient::stop()
{
    _running = false;
    std::cout << "Client closing connection" << std::endl;
    if (_socket != nullptr) {
        _socket->close();
    }
}

void RTypeClient::sendMessage(const MessageType t_type)
{
    const Message msg(t_type, m_msgSequenceNumber, 0, 1);
    const std::vector<uint8_t> msgData = msg.serialize();

    queueMessage(msg, m_serverInfo);
}

void RTypeClient::sendMessage(const MessageType t_type, uint32_t t_payload)
{
    Message msg(t_type, m_msgSequenceNumber, 0, 1);

    msg.write(t_payload);
    queueMessage(msg, m_serverInfo);
}

void RTypeClient::connectToServerRequest()
{
    sendMessage(MessageType::CONNECT);
}

void RTypeClient::disconnectFromServerRequest()
{
    sendMessage(MessageType::DISCONNECT);
    requestStop();
}

void RTypeClient::startGameRequest()
{
    sendMessage(MessageType::START_GAME);
}

void RTypeClient::lobbyStartRequest()
{
    sendMessage(MessageType::LOBBY_STATE);
}

void RTypeClient::registerHandlers()
{
    _handlers[MessageType::CONNECT_ACK] = [this](const Message& t_msg, PeerInfo& t_peerInfo) {
        handleConnectionAccepted(t_msg, t_peerInfo);
    };
    _handlers[MessageType::LOBBY_INFO] = [this](const Message& t_msg, PeerInfo& t_peerInfo) {
        handleLobbyJoint(t_msg, t_peerInfo);
    };
    _handlers[MessageType::PONG] = [this](const Message& t_msg, PeerInfo& t_peerInfo) {
        handlePongReceipt(t_msg, t_peerInfo);
    };
}
