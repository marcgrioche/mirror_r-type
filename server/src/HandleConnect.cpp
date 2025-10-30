#include "RTypeServer.hpp"
#include <iostream>

#include "Metrics.hpp"

/**
 * Handle CONNECT message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 *     peerInfo (PeerInfo&): Information about the connecting client.
 */
void RTypeServer::handleConnect(const Message& msg, PeerInfo& peerInfo)
{
    uint32_t assignedPlayerId = _nextPlayerId++;

    std::cout << "Client connected from " << peerInfo.ip_address << ":" << peerInfo.port
              << " - assigned player ID: " << assignedPlayerId << std::endl;

    if (_clients.find(assignedPlayerId) == _clients.end()) {
        _clients[assignedPlayerId] = peerInfo;
    }

    Promotheus::Metrics::IncrementConnections();
    Message connectAckMsg(MessageType::CONNECT_ACK, msg.sequence_number, assignedPlayerId);
    sendToClient(assignedPlayerId, connectAckMsg);
}
