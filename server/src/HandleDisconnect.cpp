#include "RTypeServer.hpp"
#include <iostream>

/**
 * Handle DISCONNECT message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 */
void RTypeServer::handleDisconnect(const Message& msg)
{
    std::cout << "Player " << msg.player_id << " disconnected." << std::endl;
    _clients.erase(msg.player_id);
    // TODO: Additional cleanup if needed
}
