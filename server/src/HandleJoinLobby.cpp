#include "RTypeServer.hpp"
#include <iostream>

void RTypeServer::handleJoinLobby(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo; // Unused parameter

    std::cout << "JOIN_LOBBY received from player " << msg.player_id << std::endl;

    // Read lobby ID from payload
    uint32_t lobbyId = msg.readU32();

    std::cout << "Player " << msg.player_id << " attempting to join lobby " << lobbyId << std::endl;

    bool success = _lobbyManager.joinLobby(lobbyId, msg.player_id);

    if (!success) {
        Message response(MessageType::LOBBY_INFO);
        response.sequence_number = msg.sequence_number;
        response.player_id = msg.player_id;

        // Write lobby ID (0 if join failed)
        response.write(static_cast<uint32_t>(0));
        response.write(static_cast<uint32_t>(0));
        response.write(static_cast<uint8_t>(0));
        sendToClient(msg.player_id, response);
        return;
    }
    _lobbyManager.addUsername(msg.player_id, _usernames[msg.player_id]);
    Message successResponse = _lobbyManager.initLobbyInfo(lobbyId);

    broadcastToLobby(lobbyId, successResponse);
}
