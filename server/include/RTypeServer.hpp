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
    std::string ip_address;
    uint16_t port;
    // Add more fields as needed (e.g., last sequence number)
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
    std::unordered_map<MessageType, std::function<void(const Message&, ClientInfo&)>> _handlers;

    static void handleSignal(int);

    // Message handlers
    void handleReceive(const Message& msg, const std::string& sender_ip, uint16_t sender_port);
    void handleConnect(const Message& msg, ClientInfo& clientInfo);
    void handleInput(const Message& msg, ClientInfo& clientInfo);
    void handlePing(const Message& msg, ClientInfo& clientInfo);
    void handleDisconnect(const Message& msg, ClientInfo& clientInfo);
    // ...other handlers per message type

    /**
     * Register all protocol message handlers in the handler map.
     */
    void registerHandlers();
};
