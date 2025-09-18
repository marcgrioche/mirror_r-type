#include "RTypeServer.hpp"
#include <iostream>

/**
 * Handle INPUT message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 *     peerInfo (PeerInfo&): Information about the client.
 */
void RTypeServer::handleInput(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo; // Unused parameter
    std::cout << "Received INPUT from player " << msg.player_id << std::endl;
    // TODO: Process player input
}
