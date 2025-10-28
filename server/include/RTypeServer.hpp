#pragma once

#include "DB.hpp"
#include "LobbyManager.hpp"
#include "RTypeNetwork.hpp"
#include <cstdint>
#include <memory>
#include <unordered_map>

class RTypeServer : public RTypeNetwork {
public:
    /**
     * @brief RTypeServer manages UDP communication and protocol logic for the r-type game server
     * @param port UDP port to bind the server
     */
    explicit RTypeServer(uint16_t port);

    /**
     * @brief Starts the server (begin receiving and processing messages).
     * Runs the main loop until stop is requested.
     */
    void start();

    /**
     * @brief Requests the server to stop (sets running flag to false)
     */
    void requestStop();

    /**
     * @brief Stops the server and releases resources
     */
    void stop();

    /**
     * @brief Sends a message to a specific client
     * @param playerId Target client player ID
     * @param msg Message to send
     */
    void sendToClient(uint32_t playerId, const Message& msg);

    /**
     * @brief Broadcasts a message to all connected clients
     * @param msg Message to broadcast
     */
    void broadcast(const Message& msg);

    /**
     * @brief Broadcasts a message to all players in a specific lobby
     * @param lobbyId ID of the lobby to broadcast to
     * @param msg Message to broadcast
     */
    void broadcastToLobby(uint32_t lobbyId, const Message& msg);

    /**
     * @brief Gets a reference to the database instance
     * @return Reference to the database
     */
    SqlDB::DB& getDB() { return *_db; }

private:
    uint16_t _port;
    std::unordered_map<uint32_t, PeerInfo> _clients;
    LobbyManager _lobbyManager;
    uint32_t _nextPlayerId;
    std::unordered_map<uint32_t, std::string> _usernames;
    std::unique_ptr<SqlDB::DB> _db;

    static void handleSignal(int);

    // Message handlers
    void handleConnect(const Message& msg, PeerInfo& peerInfo);
    void handleInput(const Message& msg, PeerInfo& peerInfo);
    void handlePing(const Message& msg, PeerInfo& peerInfo);
    void handleDisconnect(const Message& msg, PeerInfo& peerInfo);
    void handleCreateLobby(const Message& msg, PeerInfo& peerInfo);
    void handleJoinLobby(const Message& msg, PeerInfo& peerInfo);
    void handleStartGame(const Message& msg, PeerInfo& peerInfo);
    void handleLobbyState(const Message& msg, PeerInfo& peerInfo);
    void handleUsername(const Message& msg, PeerInfo& peerInfo);
    void handleKickPlayer(const Message& msg, PeerInfo& peerInfo);
    void handleAuthRequest(const Message& msg, PeerInfo& peerInfo);
    void removePlayerAndNotifyLobby(const Message& msg, const Lobby* t_lobby, uint32_t playerId);
    // ...other handlers per message type

    // utility methods
    bool addUsername(uint32_t playerId, const std::string& username);

    /**
     * Register all protocol message handlers in the handler map.
     */
    void registerHandlers() override;
};
