#include "RTypeServer.hpp"
#include <iostream>

void RTypeServer::handleCreateLobby(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo; // Unused parameter

    std::cout << "CREATE_LOBBY received from player " << msg.player_id << std::endl;

    uint32_t lobbyId = _lobbyManager.createLobby(msg.player_id);

    Message response = _lobbyManager.initLobbyInfo(lobbyId);

    sendToClient(msg.player_id, response);
}
