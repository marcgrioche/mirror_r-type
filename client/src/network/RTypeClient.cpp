//
// Created by dytomaomar on 9/17/25.
//

#include "RTypeClient.hpp"

#include <csignal>
#include <iostream>

void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);

using namespace Client;

RTypeClient* RTypeClient::instance = nullptr;

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

void RTypeClient::handleSignal(int)
{
    if (RTypeClient::instance) {
        RTypeClient::instance->requestStop();
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
    , m_networking(nullptr)
{
    RTypeClient::instance = this;
    _socket = std::make_unique<GameNetworkingSocket>(t_port, t_serverIpAddress, t_serverPort);
    m_networking = SteamNetworkingSockets();
    SteamNetworkingUtils()->SetGlobalCallback_SteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback);
    registerHandlers();
}

RTypeClient::RTypeClient(const uint16_t t_port, NetworkEventQueue& t_eventsQueue)
    : m_port(t_port)
    , m_debug(false)
    , m_msgSequenceNumber(0)
    , m_ping(0)
    , m_eventsQueue(t_eventsQueue)
    , m_networking(nullptr)
{
    RTypeClient::instance = this;
    _socket = std::make_unique<GameNetworkingSocket>(t_port, "", 0); // Client mode, no server initially
    m_serverInfo = { 0, "", 4242 };
    m_networking = SteamNetworkingSockets();
    SteamNetworkingUtils()->SetGlobalCallback_SteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback);
    registerHandlers();
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
    RTypeClient::instance = this;
    std::signal(SIGINT, SIG_DFL);
#endif
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
    RTypeClient::instance = nullptr;
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
    const Message msg(t_type, m_msgSequenceNumber, m_playerId, 1);
    const std::vector<uint8_t> msgData = msg.serialize();

    std::cout << "[CLIENT] Sending message type " << static_cast<int>(t_type) << " to server" << std::endl;
    queueMessage(msg, m_serverInfo);
}

void RTypeClient::sendMessage(const MessageType t_type, uint32_t t_payload)
{
    Message msg(t_type, m_msgSequenceNumber, m_playerId, 1);

    msg.write(t_payload);
    queueMessage(msg, m_serverInfo);
}

void RTypeClient::sendMessage(const Message& t_msg)
{
    Message msg(t_msg.getType(), t_msg.getPayload(), t_msg.sequence_number, m_playerId, t_msg.version);

    if (isGameCriticalMessage(t_msg.getType())) {
        // Send via P2P to all connected peers, fallback to server
        bool sentViaP2P = false;
        for (auto& pair : m_p2pConnected) {
            if (pair.second) {
                sendViaP2P(pair.first, msg);
                sentViaP2P = true;
            }
        }
        if (!sentViaP2P) {
            queueMessage(msg, m_serverInfo);
        }
    } else {
        // Send via server
        queueMessage(msg, m_serverInfo);
    }
}

void RTypeClient::sendMessageImmediately(const Message& t_msg)
{
    Message msg(t_msg.getType(), t_msg.getPayload(), t_msg.sequence_number, m_playerId, t_msg.version);
    std::vector<uint8_t> data = msg.serialize();

    if (_socket) {
        _socket->send(data, m_serverInfo.ip_address, m_serverInfo.port);
    }
}

void RTypeClient::connectToServerRequest()
{
    std::cout << "[CLIENT] Sending CONNECT message to server at " << m_serverInfo.ip_address << ":" << m_serverInfo.port << std::endl;
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

void RTypeClient::handleGameState(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;
    m_eventsQueue.push(NetworkEvent { MessageType::GAME_STATE, t_msg });
}

void RTypeClient::handleGameEndWin(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;
    m_eventsQueue.push(NetworkEvent { MessageType::GAME_END_WIN, t_msg });
}

void RTypeClient::handleGameEndLose(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;
    m_eventsQueue.push(NetworkEvent { MessageType::GAME_END_LOSE, t_msg });
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
    _handlers[MessageType::SPAWN_ENTITY] = [this](const Message& t_msg, PeerInfo& t_peerInfo) {
        handleSpawnEntity(t_msg, t_peerInfo);
    };
    _handlers[MessageType::DESPAWN_ENTITY] = [this](const Message& t_msg, PeerInfo& t_peerInfo) {
        handleDespawnEntity(t_msg, t_peerInfo);
    };
    _handlers[MessageType::GAME_STATE] = [this](const Message& t_msg, PeerInfo& t_peerInfo) {
        handleGameState(t_msg, t_peerInfo);
    };
    _handlers[MessageType::GAME_END_WIN] = [this](const Message& t_msg, PeerInfo& t_peerInfo) {
        handleGameEndWin(t_msg, t_peerInfo);
    };
    _handlers[MessageType::GAME_END_LOSE] = [this](const Message& t_msg, PeerInfo& t_peerInfo) {
        handleGameEndLose(t_msg, t_peerInfo);
    };
    _handlers[MessageType::USERNAME_ACK] = [this](const Message& t_msg, PeerInfo& t_peerInfo) {
        handleUsernameRequestState(t_msg, t_peerInfo);
    };
    _handlers[MessageType::PEER_LIST] = [this](const Message& t_msg, PeerInfo& t_peerInfo) {
        handlePeerList(t_msg, t_peerInfo);
    };
}

void RTypeClient::handlePeerList(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;
    t_msg.resetReadPosition();

    uint32_t numPeers = t_msg.readU32();
    for (uint32_t i = 0; i < numPeers; ++i) {
        uint32_t peerId = t_msg.readU32();
        std::string ip = t_msg.readString(15); // Assuming max IP length
        uint16_t port = t_msg.readU16();

        PeerInfo peerInfo = { peerId, ip, port };
        m_p2pPeers[peerId] = peerInfo;
        establishP2PConnection(peerId, peerInfo);
    }
}

void RTypeClient::establishP2PConnection(uint32_t peerId, const PeerInfo& peerInfo)
{
    if (!m_networking)
        return;

    SteamNetworkingIPAddr addr;
    addr.Clear();
    addr.ParseString(peerInfo.ip_address.c_str());
    addr.m_port = peerInfo.port;

    SteamNetworkingConfigValue_t opt;
    opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)this);

    HSteamNetConnection conn = m_networking->ConnectByIPAddress(addr, 1, &opt);
    if (conn != k_HSteamNetConnection_Invalid) {
        m_p2pConnections[peerId] = conn;
        m_p2pConnected[peerId] = false; // Will be set to true on successful connection
    } else {
        std::cerr << "Failed to initiate P2P connection to peer " << peerId << std::endl;
    }
}

bool RTypeClient::isP2PAvailable(uint32_t peerId) const
{
    auto it = m_p2pConnected.find(peerId);
    return it != m_p2pConnected.end() && it->second;
}

void RTypeClient::sendViaP2P(uint32_t peerId, const Message& msg)
{
    if (!isP2PAvailable(peerId))
        return;

    auto connIt = m_p2pConnections.find(peerId);
    if (connIt == m_p2pConnections.end())
        return;

    std::vector<uint8_t> data = msg.serialize();
    EResult res = m_networking->SendMessageToConnection(connIt->second, data.data(), data.size(), k_nSteamNetworkingSend_Reliable, nullptr);
    if (res != k_EResultOK) {
        std::cerr << "Failed to send P2P message to peer " << peerId << std::endl;
    }
}

void RTypeClient::sendViaServer(const Message& msg)
{
    queueMessage(msg, m_serverInfo);
}

bool RTypeClient::isGameCriticalMessage(MessageType type) const
{
    // Define game-critical messages that should prefer P2P
    return type == MessageType::INPUT; // Example: input messages are critical for low latency
}

void RTypeClient::handleP2PConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    // Update connection status
    uint32_t peerId = 0; // Need to map connection handle to peerId
    // This is simplified; in practice, maintain a reverse map
    for (auto& pair : m_p2pConnections) {
        if (pair.second == pInfo->m_hConn) {
            peerId = pair.first;
            break;
        }
    }

    if (peerId != 0) {
        if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting && pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_Connected) {
            m_p2pConnected[peerId] = true;
            std::cout << "P2P connection established to peer " << peerId << std::endl;
        } else if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer || pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally) {
            m_p2pConnected[peerId] = false;
            std::cout << "P2P connection lost to peer " << peerId << std::endl;
        }
    }
}

// Static callback function
void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    // Assuming instance is set
    if (RTypeClient::instance) {
        RTypeClient::instance->handleP2PConnectionStatusChanged(pInfo);
    }
}
