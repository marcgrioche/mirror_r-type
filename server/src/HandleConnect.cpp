#include "RTypeServer.hpp"
#include <iostream>

/**
 * Handle CONNECT message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 */
void RTypeServer::handleConnect(const Message& msg)
{
    // Handle CONNECT message (example logic)
    std::cout << "Client " << msg.player_id << " connected." << std::endl;
    if (_clients.find(msg.player_id) == _clients.end()) {
        _clients[msg.player_id] = ClientInfo { msg.player_id };
    }
    // Send CONNECT_ACK (not implemented here)
}
