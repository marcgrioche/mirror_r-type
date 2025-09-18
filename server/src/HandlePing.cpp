#include "RTypeServer.hpp"
#include <iostream>

/**
 * Handle PING message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 *     peerInfo (PeerInfo&): Information about the client.
 */
void RTypeServer::handlePing(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo; // Unused parameter
    std::cout << "Received PING from player " << msg.player_id << std::endl;
    Message pongMsg(MessageType::PONG, msg.sequence_number, msg.player_id);
    sendToClient(msg.player_id, pongMsg);
}
