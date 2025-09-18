#pragma once

#ifndef RTYPENETWORK_HPP
#define RTYPENETWORK_HPP

#include "Message.hpp"
#include "UdpSocket.hpp"
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
     * RTypeNetwork provides common UDP networking functionality for both server and client.
     */
    RTypeNetwork();

    virtual ~RTypeNetwork() = default;

    /**
     * Start the network node (begin receiving and processing messages).
     * Implementation differs between server (bind to port) and client (connect to server).
     */
    virtual void start() = 0;

    /**
     * Request the network node to stop.
     */
    virtual void requestStop();

    /**
     * Stop the network node and release resources.
     */
    virtual void stop() = 0;

    /**
     * Send a message to a specific peer (queues the message).
     *
     * Args:
     *     peerInfo (const PeerInfo&): Target peer information.
     *     msg (const Message&): Message to send.
     */
    void sendToPeer(const PeerInfo& peerInfo, const Message& msg);

    /**
     * Send a message to a specific IP and port.
     *
     * Args:
     *     ip (const std::string&): Target IP address.
     *     port (uint16_t): Target port.
     *     msg (const Message&): Message to send.
     */
    void sendTo(const std::string& ip, uint16_t port, const Message& msg);

protected:
    std::unique_ptr<UdpSocket> _socket;
    std::queue<QueuedMessage> _outgoingQueue;
    std::atomic<bool> _running { true };

    // Handler map for event dispatch
    std::unordered_map<MessageType, std::function<void(const Message&, PeerInfo&)>> _handlers;

    /**
     * Register protocol message handlers in the handler map.
     * Must be implemented by subclasses with their specific handlers.
     */
    virtual void registerHandlers() = 0;

    /**
     * Handle incoming message reception and dispatch to appropriate handler.
     *
     * Args:
     *     msg (const Message&): Received message.
     *     sender_ip (const std::string&): Sender IP address.
     *     sender_port (uint16_t): Sender port.
     */
    void handleReceive(const Message& msg, const std::string& sender_ip, uint16_t sender_port);

    /**
     * Process incoming messages when socket has data.
     * Called by subclasses in their main loop.
     */
    void processIncomingMessages();

    /**
     * Create PeerInfo from message and sender details.
     *
     * Args:
     *     msg (const Message&): Message containing peer ID.
     *     sender_ip (const std::string&): Sender IP.
     *     sender_port (uint16_t): Sender port.
     *
     * Returns:
     *     PeerInfo: Constructed peer information.
     */
    PeerInfo createPeerInfo(const Message& msg, const std::string& sender_ip, uint16_t sender_port);

    /**
     * Queue a message for sending to a peer.
     *
     * Args:
     *     msg (const Message&): Message to queue.
     *     peerInfo (const PeerInfo&): Target peer information.
     */
    void queueMessage(const Message& msg, const PeerInfo& peerInfo);

    /**
     * Process outgoing messages when socket is ready for writing.
     */
    void processOutgoingMessages();
};

#endif // RTYPENETWORK_HPP
