#include "RTypeServer.hpp"
#include <iostream>

void RTypeServer::handleLobbyState(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo; // Unused parameter

    std::cout << "LOBBY_STATE received from player " << msg.player_id << std::endl;

    uint32_t playerLobby = _lobbyManager.getPlayerLobby(msg.player_id);

    Message response(MessageType::LOBBY_INFO);
    response.sequence_number = msg.sequence_number;
    response.player_id = msg.player_id;

    response.write(playerLobby); // Lobby ID (0 = not in lobby)

    if (playerLobby != 0) {
        const Lobby* lobby = _lobbyManager.getLobby(playerLobby);
        if (lobby) {
            response.write(static_cast<uint8_t>(lobby->state)); // Lobby state
        } else {
            response.write(static_cast<uint8_t>(0)); // Default WAITING state
        }
    } else {
        response.write(static_cast<uint8_t>(0)); // Not in lobby
    }

    sendToClient(msg.player_id, response);
}
