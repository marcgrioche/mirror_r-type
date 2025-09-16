#pragma once

#include "Message.hpp"
#include <cstdint>
#include <memory>
#include <unordered_map>

struct ClientInfo {
    uint32_t player_id;
    // Add more fields as needed (e.g., endpoint, last sequence number)
};

class RTypeServer {
public:
    /**
     * RTypeServer manages UDP communication and protocol logic for the r-type game server.
     *
     * Args:
     *     port (uint16_t): UDP port to bind the server.
     */
    explicit RTypeServer(uint16_t port);

    /**
     * Start the server (begin receiving and processing messages).
     */
    void start();

    /**
     * Stop the server and release resources.
     */
    void stop();

    /**
     * Send a message to a specific client.
     *
     * Args:
     *     playerId (uint32_t): Target client player ID.
     *     msg (const Message&): Message to send.
     */
    void sendToClient(uint32_t playerId, const Message& msg);

    /**
     * Broadcast a message to all connected clients.
     *
     * Args:
     *     msg (const Message&): Message to broadcast.
     */
    void broadcast(const Message& msg);

private:
    uint16_t port_;
    std::unordered_map<uint32_t, ClientInfo> clients_;

    // Add socket and protocol handling members here (e.g., UdpSocket)
    // Add extensibility hooks for game logic

    // Message handlers
    void handleReceive(const Message& msg);
    void handleConnect(const Message& msg);
    // ...other handlers per message type
};
