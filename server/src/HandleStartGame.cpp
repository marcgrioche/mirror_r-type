#include "RTypeServer.hpp"
#include <iostream>

void RTypeServer::handleStartGame(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo; // Unused parameter

    std::cout << "START_GAME received from player " << msg.player_id << std::endl;

    uint32_t playerLobby = _lobbyManager.getPlayerLobby(msg.player_id);
    if (playerLobby == 0) {
        std::cout << "Player " << msg.player_id << " is not in any lobby" << std::endl;
        return;
    }

    bool success = _lobbyManager.startGame(playerLobby, msg.player_id);
    if (success) {
        std::cout << "Game started successfully in lobby " << playerLobby << std::endl;
        // TODO: Start game thread when GameInstance is implemented
    } else {
        std::cout << "Failed to start game in lobby " << playerLobby << std::endl;
    }
}
