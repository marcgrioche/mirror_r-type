#include "RTypeServer.hpp"
#include <iostream>

/**
 * Handle INPUT message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 */
void RTypeServer::handleInput(const Message& msg)
{
    std::cout << "Received INPUT from player " << msg.player_id << std::endl;
    // TODO: Process player input
}
