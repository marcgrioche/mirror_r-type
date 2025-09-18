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
    Message pongMsg(MessageType::PONG, msg.sequence_number, msg.player_id);
    queueMessage(pongMsg, msg.player_id);
}
