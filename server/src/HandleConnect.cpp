#include "RTypeServer.hpp"
#include <iostream>

/**
 * Handle CONNECT message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 *     peerInfo (PeerInfo&): Information about the connecting client.
 */
void RTypeServer::handleConnect(const Message& msg, PeerInfo& peerInfo)
{
    std::cout << "Client " << msg.player_id << " connected from " << peerInfo.ip_address << ":" << peerInfo.port << std::endl;
    if (_clients.find(msg.player_id) == _clients.end()) {
        _clients[msg.player_id] = peerInfo;
    }
    Message connectAckMsg(MessageType::CONNECT_ACK, msg.sequence_number, msg.player_id);
    sendToClient(msg.player_id, connectAckMsg);
}
