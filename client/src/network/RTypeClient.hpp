#pragma once
#include "RTypeNetwork.hpp"
#include <atomic>
#include <cstdint>
#include <memory>
#include <queue>
#include <unordered_map>

#include "../../../shared/include/GameInputs.hpp"
#include "NetworkEventQueue.hpp"
#include "managers/InputManager.hpp"
#include <steam/steamnetworkingsockets.h>

namespace Client {
class RTypeClient : public RTypeNetwork {
public:
    /**
     * @brief Constructs an RTypeClient with specified port and event queue
     * @param t_port The port to bind the client to
     * @param t_eventsQueue Reference to the network event queue for handling events
     */
    explicit RTypeClient(uint16_t t_port, NetworkEventQueue& t_eventsQueue);

    /**
     * @brief Constructs an RTypeClient with server info, port, and event queue
     * @param t_serverIpAddress The server IP address to connect to
     * @param t_serverPort The server port to connect to
     * @param t_port The port to bind the client to
     * @param t_eventsQueue Reference to the network event queue for handling events
     */
    RTypeClient(const std::string& t_serverIpAddress, uint16_t t_serverPort, uint16_t t_port,
        NetworkEventQueue& t_eventsQueue);

    /**
     * @brief Starts the client network operations
     */
    void start() override;

    /**
     * @brief Stops the client network operations
     */
    void stop() override;

    /**
     * @brief Requests the client to stop network operations
     */
    void requestStop() override;

    // setters

    /**
     * @brief Sets the server IP address and port for connection
     * @param t_serverIpAddress The server IP address
     * @param t_serverPort The server port number
     */
    void setServerInfo(const std::string& t_serverIpAddress, uint16_t t_serverPort);

    /**
     * @brief Enables or disables debug mode
     * @param t_debugMode True to enable debug mode, false to disable
     */
    void setDebugMode(bool t_debugMode);

    /**
     * @brief Static signal handler for system signals
     * @param t_signal The signal number received
     */
    static void handleSignal(int t_signal);

    // server communication functions

    /**
     * @brief Sends a message with the specified type
     * @param t_type The message type to send
     */
    void sendMessage(MessageType t_type);

    /**
     * @brief Sends a message with type and payload
     * @param t_type The message type to send
     * @param t_payload The payload data to include
     */
    void sendMessage(MessageType t_type, uint32_t t_payload);

    /**
     * @brief Sends a pre-constructed message
     * @param t_msg The message to send
     */
    void sendMessage(const Message& t_msg);

    /**
     * @brief Sends a message immediately without queuing
     * @param t_msg The message to send immediately
     */
    void sendMessageImmediately(const Message& t_msg);

    /**
     * @brief Sends a connection request to the server
     */
    void connectToServerRequest();

    /**
     * @brief Sends a disconnect request to the server
     */
    void disconnectFromServerRequest();

    /**
     * @brief Sends a lobby creation request
     */
    void createLobbyRequest();

    /**
     * @brief Sends a username creation request
     * @param t_username The username to set
     */
    void createUsernameRequest(const std::string& t_username);

    /**
     * @brief Sends a lobby join request
     * @param t_lobbyId The ID of the lobby to join
     */
    void joinLobbyRequest(uint32_t t_lobbyId);

    /**
     * @brief Sends a game start request
     */
    void startGameRequest();

    /**
     * @brief Sends a lobby start request
     */
    void lobbyStartRequest();

    /**
     * @brief Sends a ping request to measure latency
     */
    void pingRequest();

    /**
     * @brief Handles input from the input manager
     * @param t_inputs The input manager containing current input state
     */
    void handleInputs(const InputManager& t_inputs);

    /**
     * @brief Handles all inputs from the input manager
     * @param t_inputs The input manager containing current input state
     */
    void handleAllInputs(const InputManager& t_inputs);

    /**
     * @brief Sends the current input state to the server
     * @param inputs Vector of input pairs (GameInput, isPressed)
     */
    void sendCurrentInputState(const std::vector<std::pair<GameInput, bool>>& inputs);

    /**
     * @brief Gets the current game tick
     * @return The current tick number
     */
    uint32_t getCurrentTick() const { return m_currentTick; }

    /**
     * @brief Increments the current tick counter
     */
    void incrementTick() { m_currentTick++; }

    /**
     * @brief Gets the player's ID
     * @return The player's unique identifier
     */
    uint32_t getPlayerId() const { return m_playerId; }

    // P2P methods
    void handleP2PConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

public:
    static RTypeClient* instance; // For callbacks

private:
    void registerHandlers() override;
    uint16_t m_port;
    PeerInfo m_serverInfo;
    bool m_debug;
    uint16_t m_msgSequenceNumber;
    uint32_t m_playerId = 0;
    uint32_t m_lobbyId = 0;
    uint64_t m_ping;
    uint32_t m_currentTick = 0;
    bool m_isConnected = false;
    NetworkEventQueue& m_eventsQueue;
    std::unordered_map<GameAction, bool> m_previousInputStates;

    // P2P connection management
    std::unordered_map<uint32_t, PeerInfo> m_p2pPeers; // peer_id to peer info
    std::unordered_map<uint32_t, HSteamNetConnection> m_p2pConnections; // peer_id to connection handle
    std::unordered_map<uint32_t, bool> m_p2pConnected; // peer_id to connection status
    ISteamNetworkingSockets* m_networking; // GNS interface for P2P

    // P2P methods
    void establishP2PConnection(uint32_t peerId, const PeerInfo& peerInfo);
    bool isP2PAvailable(uint32_t peerId) const;
    void sendViaP2P(uint32_t peerId, const Message& msg);
    void sendViaServer(const Message& msg);
    bool isGameCriticalMessage(MessageType type) const;

    // server responses handlers
    void handleConnectionAccepted(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleLobbyCreation(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleLobbyJoint(const Message& t_msg, PeerInfo& t_peerInfo);
    void handlePongReceipt(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleSpawnEntity(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleDespawnEntity(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleGameState(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleGameEndWin(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleGameEndLose(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleUsernameRequestState(const Message& t_msg, PeerInfo& t_peerInfo);
    void handlePeerList(const Message& t_msg, PeerInfo& t_peerInfo);
};
}
