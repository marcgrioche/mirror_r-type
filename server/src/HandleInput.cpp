#include "RTypeServer.hpp"
#include <iostream>

/**
 * Handle INPUT message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 *     clientInfo (ClientInfo&): Information about the client.
 */
void RTypeServer::handleInput(const Message& msg, ClientInfo& clientInfo)
{
    (void)clientInfo; // Unused parameter
    std::cout << "Received INPUT from player " << msg.player_id << std::endl;
    // TODO: Process player input
}
