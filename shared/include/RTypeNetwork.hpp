#pragma once

#ifndef RTYPENETWORK_HPP
#define RTYPENETWORK_HPP

#include "GameNetworkingSocket.hpp"
#include "Message.hpp"
#include <atomic>
#include <functional>
#include <memory>
#include <queue>
#include <unordered_map>

struct PeerInfo {
    uint32_t peer_id;
    std::string ip_address;
    uint16_t port;
    // Add more fields as needed
};

struct QueuedMessage {
    Message msg;
    PeerInfo peerInfo;
};

class RTypeNetwork {
public:
    /**
     * @brief RTypeNetwork provides common UDP networking functionality for both server and client
     */
    RTypeNetwork();

    /**
     * @brief Virtual destructor for proper cleanup
     */
    virtual ~RTypeNetwork() = default;

    /**
     * @brief Starts the network node (begin receiving and processing messages).
     * Implementation differs between server (bind to port) and client (connect to server).
     */
    virtual void start() = 0;

    /**
     * @brief Requests the network node to stop
     */
    virtual void requestStop();

    /**
     * @brief Stops the network node and releases resources
     */
    virtual void stop() = 0;

    /**
     * @brief Sends a message to a specific peer (queues the message)
     * @param peerInfo Target peer information
     * @param msg Message to send
     */
    void sendToPeer(const PeerInfo& peerInfo, const Message& msg);

    /**
     * @brief Sends a message to a specific IP and port
     * @param ip Target IP address
     * @param port Target port
     * @param msg Message to send
     */
    void sendTo(const std::string& ip, uint16_t port, const Message& msg);

protected:
    std::unique_ptr<GameNetworkingSocket> _socket;
    std::queue<QueuedMessage> _outgoingQueue;
    std::atomic<bool> _running { true };

    // Handler map for event dispatch
    std::unordered_map<MessageType, std::function<void(const Message&, PeerInfo&)>> _handlers;

    /**
     * @brief Registers protocol message handlers in the handler map.
     * Must be implemented by subclasses with their specific handlers.
     */
    virtual void registerHandlers() = 0;

    /**
     * @brief Handles incoming message reception and dispatches to appropriate handler
     * @param msg Received message
     * @param sender_ip Sender IP address
     * @param sender_port Sender port
     */
    void handleReceive(const Message& msg, const std::string& sender_ip, uint16_t sender_port);

    /**
     * @brief Processes incoming messages when socket has data.
     * Called by subclasses in their main loop.
     */
    void processIncomingMessages();

    /**
     * @brief Creates PeerInfo from message and sender details
     * @param msg Message containing peer ID
     * @param sender_ip Sender IP
     * @param sender_port Sender port
     * @return Constructed peer information
     */
    PeerInfo createPeerInfo(const Message& msg, const std::string& sender_ip, uint16_t sender_port);

    /**
     * @brief Queues a message for sending to a peer
     * @param msg Message to queue
     * @param peerInfo Target peer information
     */
    void queueMessage(const Message& msg, const PeerInfo& peerInfo);

    /**
     * @brief Processes outgoing messages when socket is ready for writing
     */
    void processOutgoingMessages();
};

#endif // RTYPENETWORK_HPP
