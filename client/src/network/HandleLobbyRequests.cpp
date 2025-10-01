#include "RTypeClient.hpp"
#include <iostream>

using namespace Client;

// requests handlers
void RTypeClient::createLobbyRequest()
{
    sendMessage(MessageType::CREATE_LOBBY);
}

void RTypeClient::joinLobbyRequest(const uint32_t t_lobbyId)
{
    sendMessage(MessageType::JOIN_LOBBY, t_lobbyId);
}

// responses handlers
void RTypeClient::handleLobbyCreation(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo; // unused parameter
    const uint32_t lobbyId = t_msg.readU32();

    m_lobbyId = lobbyId;
    std::cout << "Lobby " << lobbyId << " created" << std::endl;
}

void RTypeClient::handleLobbyJoint(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;
    const uint32_t lobbyId = t_msg.readU32();

    if (lobbyId == 0) {
        std::cout << "Lobby not found" << std::endl;
    } else {
        m_lobbyId = lobbyId;
        std::cout << "Joined lobby " << lobbyId << std::endl;
    }

    m_eventsQueue.push({ MessageType::LOBBY_INFO, t_msg });
}
