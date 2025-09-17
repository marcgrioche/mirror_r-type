#include "RTypeServer.hpp"
#include <iostream>

/**
 * Handle CONNECT message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 *     clientInfo (ClientInfo&): Information about the connecting client.
 */
void RTypeServer::handleConnect(const Message& msg, ClientInfo& clientInfo)
{
    // Handle CONNECT message (example logic)
    std::cout << "Client " << msg.player_id << " connected from " << clientInfo.ip_address << ":" << clientInfo.port << std::endl;
    if (_clients.find(msg.player_id) == _clients.end()) {
        _clients[msg.player_id] = clientInfo;
    }
    // Send CONNECT_ACK (not implemented here)
}
