#include <chrono>

#include "RTypeClient.hpp"
#include <iostream>

using namespace Client;

void RTypeClient::handleConnectionAccepted(const Message& t_msg, PeerInfo& t_peerInfo)
{
    m_playerId = t_msg.player_id;
    m_isConnected = true;
    std::cout << "[CLIENT] Connected to server - assigned player ID: " << m_playerId << std::endl;

    m_eventsQueue.push({ MessageType::CONNECT_ACK, true });
    if (m_serverInfo.ip_address != t_peerInfo.ip_address || m_serverInfo.port != t_peerInfo.port) {
        m_serverInfo = t_peerInfo;
    }
    pingRequest();
}

void RTypeClient::createAuthenticationRequest(const std::string& t_username, const std::string& t_password)
{
    Message msg(MessageType::AUTH_REQUEST, m_msgSequenceNumber, m_playerId, 1);

    msg.write(t_username);
    msg.write(t_password);
    sendMessage(msg);
}

void RTypeClient::pingRequest()
{
    sendMessage(MessageType::PING);
    m_ping = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

void RTypeClient::handlePongReceipt(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_msg;
    (void)t_peerInfo;

    const auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    m_ping = now - m_ping;
}

void RTypeClient::handleSpawnEntity(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;

    // Push the message to the event queue for Game to process
    m_eventsQueue.push({ MessageType::SPAWN_ENTITY, t_msg });
}

void RTypeClient::handleDespawnEntity(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;

    // Push the message to the event queue for Game to process
    m_eventsQueue.push({ MessageType::DESPAWN_ENTITY, t_msg });
}

void RTypeClient::handleUsernameRequestState(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;
    Message msg = t_msg;
    msg.resetReadPosition();
    const auto state = msg.readU8();

    bool result = state == 1;
    m_eventsQueue.push({ MessageType::USERNAME_ACK, result });
}

void RTypeClient::handleAuthenticationResponse(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;
    Message msg = t_msg;
    msg.resetReadPosition();
    const auto state = msg.readU32();

    if (state) {
        m_playerId = state;
    }
    m_eventsQueue.push({ MessageType::AUTH_RESPONSE, t_msg });
}
