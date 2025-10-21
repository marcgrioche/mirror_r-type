#include "RTypeServer.hpp"
#include <algorithm>
#include <iostream>

/**
 * Handle DISCONNECT message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 *     peerInfo (PeerInfo&): Information about the client.
 */
void RTypeServer::handleDisconnect(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo; // Unused parameter
    std::cout << "Player " << msg.player_id << " disconnected." << std::endl;

    uint32_t lobbyId = _lobbyManager.getPlayerLobby(msg.player_id);
    _lobbyManager.removePlayer(msg.player_id);

    // Remove from peer list and send updated list
    if (lobbyId != 0) {
        auto& peers = _peerLists[lobbyId];
        peers.erase(std::remove_if(peers.begin(), peers.end(),
                        [msg](const PeerInfo& p) { return p.peer_id == msg.player_id; }),
            peers.end());
        sendPeerListToLobby(lobbyId);
    }

    _clients.erase(msg.player_id);
}
