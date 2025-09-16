#pragma once

#include "Message.hpp"
#include "UdpSocket.hpp"
#include <atomic>
#include <cstdint>
#include <functional>
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
    std::unique_ptr<UdpSocket> _socket;
    std::unordered_map<uint32_t, ClientInfo> _clients;
    std::atomic<bool> _running { true };

    // Handler map for event dispatch
    std::unordered_map<MessageType, std::function<void(const Message&)>> _handlers;

    static void handleSignal(int);

    // Message handlers
    void handleReceive(const Message& msg);
    void handleConnect(const Message& msg);
    void handleInput(const Message& msg);
    void handlePing(const Message& msg);
    void handleDisconnect(const Message& msg);
    // ...other handlers per message type

    /**
     * Register all protocol message handlers in the handler map.
     */
    void registerHandlers();
};
