#pragma once

#include "RTypeNetwork.hpp"
#include <cstdint>
#include <unordered_map>

class RTypeServer : public RTypeNetwork {
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
     * Runs the main loop until stop is requested.
     */
    void start();

    /**
     * Request the server to stop (sets running flag to false).
     */
    void requestStop();

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
    uint16_t _port;
    std::unordered_map<uint32_t, PeerInfo> _clients;

    static void handleSignal(int);

    // Message handlers
    void handleConnect(const Message& msg, PeerInfo& peerInfo);
    void handleInput(const Message& msg, PeerInfo& peerInfo);
    void handlePing(const Message& msg, PeerInfo& peerInfo);
    void handleDisconnect(const Message& msg, PeerInfo& peerInfo);
    // ...other handlers per message type

    /**
     * Register all protocol message handlers in the handler map.
     */
    void registerHandlers() override;
};
