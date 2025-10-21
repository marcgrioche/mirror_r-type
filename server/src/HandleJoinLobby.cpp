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

    Message response(MessageType::LOBBY_INFO);
    response.sequence_number = msg.sequence_number;
    response.player_id = msg.player_id;

    // Write lobby ID (0 if join failed)
    response.write(success ? lobbyId : static_cast<uint32_t>(0));

    sendToClient(msg.player_id, response);

    if (success) {
        // Add peer to peer list for this lobby
        auto it = _clients.find(msg.player_id);
        if (it != _clients.end()) {
            _peerLists[lobbyId].push_back(it->second);
        }

        // Send updated peer list to all players in the lobby
        sendPeerListToLobby(lobbyId);
    }
}
