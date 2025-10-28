#include "RTypeServer.hpp"
#include <iostream>

void RTypeServer::handleCreateLobby(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo; // Unused parameter

    std::cout << "CREATE_LOBBY received from player " << msg.player_id << std::endl;

    if (_lobbyManager.getPlayerLobby(msg.player_id) != 0) {
        _lobbyManager.removePlayer(msg.player_id);
    }

    uint32_t lobbyId = _lobbyManager.createLobby(msg.player_id);

    if (lobbyId) {
        const bool state = _lobbyManager.addUsername(msg.player_id, _usernames[msg.player_id]);
        if (state) {
            std::cout << "Username added " << _usernames[msg.player_id] << " to the lobby" << std::endl;
        } else {
            std::cout << "Username not added to the lobby" << std::endl;
        }
    }
    Message response = _lobbyManager.initLobbyInfo(lobbyId);

    sendToClient(msg.player_id, response);
}
