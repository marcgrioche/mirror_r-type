#include "RTypeServer.hpp"
#include <iostream>

void RTypeServer::handleLobbyState(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo; // Unused parameter

    std::cout << "LOBBY_STATE received from player " << msg.player_id << std::endl;

    uint32_t playerLobby = _lobbyManager.getPlayerLobby(msg.player_id);

    Message response = _lobbyManager.initLobbyInfo(playerLobby);
    response.sequence_number = msg.sequence_number;
    response.player_id = msg.player_id;

    sendToClient(msg.player_id, response);
}
