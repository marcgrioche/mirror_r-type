#include <chrono>

#include "RTypeClient.hpp"
#include <iostream>

using namespace Client;

void RTypeClient::handleConnectionAccepted(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_msg;
    m_eventsQueue.push({ MessageType::CONNECT_ACK, true });
    if (m_serverInfo.ip_address != t_peerInfo.ip_address || m_serverInfo.port != t_peerInfo.port) {
        m_serverInfo = t_peerInfo;
    }
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
