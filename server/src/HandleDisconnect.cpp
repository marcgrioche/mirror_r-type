#include "RTypeServer.hpp"
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
    _clients.erase(msg.player_id);
    // TODO: Additional cleanup if needed
}
