#include "RTypeServer.hpp"
#include <iostream>

/**
 * Handle PING message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 *     clientInfo (ClientInfo&): Information about the client.
 */
void RTypeServer::handlePing(const Message& msg, ClientInfo& clientInfo)
{
    (void)clientInfo; // Unused parameter
    std::cout << "Received PING from player " << msg.player_id << std::endl;
    // TODO: Respond with PONG if needed
}
