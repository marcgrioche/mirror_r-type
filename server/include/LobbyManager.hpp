#pragma once

#include "../../shared/include/GameInstance.hpp"
#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

enum class LobbyState {
    WAITING,
    RUNNING,
    FINISHED
};

struct PlayerInput {
    uint32_t playerId;
    uint32_t tick;
    std::vector<std::pair<GameInput, bool>> inputs;
};

struct Lobby {
    uint32_t id;
    uint32_t creatorId;
    std::vector<uint32_t> players;
    LobbyState state;
    uint32_t maxPlayers;
    std::unique_ptr<GameInstance> gameInstance;
    std::thread gameThread;
    std::atomic<bool> threadRunning;
    std::queue<PlayerInput> inputQueue;
    std::mutex inputMutex;
    std::unordered_set<uint32_t> readyPlayers;

    Lobby(uint32_t lobbyId, uint32_t creator);
    ~Lobby();

    void queueInput(const PlayerInput& input);
    bool hasPendingInputs() const;
    PlayerInput dequeueInput();
};

class RTypeServer; // Forward declaration

class LobbyManager {
public:
    /**
     * LobbyManager handles creation, management, and state tracking of game lobbies.
     * Thread-safe for concurrent access from multiple threads.
     */
    LobbyManager();

    /**
     * Set the server reference for broadcasting messages.
     *
     * Args:
     *     server (RTypeServer*): Pointer to the server instance
     */
    void setServer(RTypeServer* server);

    /**
     * Create a new lobby with the specified creator.
     *
     * Args:
     *     creatorId (uint32_t): Player ID of the lobby creator
     *
     * Returns:
     *     uint32_t: Unique lobby ID, or 0 if creation failed
     */
    uint32_t createLobby(uint32_t creatorId);

    /**
     * Join an existing lobby.
     *
     * Args:
     *     lobbyId (uint32_t): ID of the lobby to join
     *     playerId (uint32_t): Player ID wanting to join
     *
     * Returns:
     *     bool: True if join was successful, false otherwise
     */
    bool joinLobby(uint32_t lobbyId, uint32_t playerId);

    /**
     * Start a game in the specified lobby (only lobby creator can do this).
     *
     * Args:
     *     lobbyId (uint32_t): ID of the lobby to start
     *     playerId (uint32_t): Player ID attempting to start (must be creator)
     *
     * Returns:
     *     bool: True if game start was successful, false otherwise
     */
    bool startGame(uint32_t lobbyId, uint32_t playerId);

    /**
     * Get lobby information.
     *
     * Args:
     *     lobbyId (uint32_t): ID of the lobby to query
     *
     * Returns:
     *     const Lobby*: Pointer to lobby info, or nullptr if not found
     */
    const Lobby* getLobby(uint32_t lobbyId) const;

    /**
     * Get the lobby ID that a player is currently in.
     *
     * Args:
     *     playerId (uint32_t): Player ID to check
     *
     * Returns:
     *     uint32_t: Lobby ID, or 0 if player is not in any lobby
     */
    uint32_t getPlayerLobby(uint32_t playerId) const;

    /**
     * Remove a player from their current lobby.
     *
     * Args:
     *     playerId (uint32_t): Player ID to remove
     *
     * Returns:
     *     bool: True if player was removed, false if not in any lobby
     */
    bool removePlayer(uint32_t playerId);

    /**
     * Get all active lobbies.
     *
     * Returns:
     *     std::vector<uint32_t>: List of active lobby IDs
     */
    std::vector<uint32_t> getActiveLobbies() const;

    /**
     * Get the GameInstance for a lobby.
     *
     * Args:
     *     lobbyId (uint32_t): ID of the lobby
     *
     * Returns:
     *     GameInstance*: Pointer to the game instance, or nullptr if not running
     */
    GameInstance* getGameInstance(uint32_t lobbyId);

    /**
     * Get the players in a lobby (thread-safe).
     *
     * Args:
     *     lobbyId (uint32_t): ID of the lobby
     *
     * Returns:
     *     std::vector<uint32_t>: List of player IDs in the lobby
     */
    std::vector<uint32_t> getLobbyPlayers(uint32_t lobbyId) const;

private:
    std::unordered_map<uint32_t, std::unique_ptr<Lobby>> _lobbies;
    std::unordered_map<uint32_t, uint32_t> _playerToLobby; // playerId -> lobbyId
    mutable std::mutex _mutex;
    uint32_t _nextLobbyId;
    RTypeServer* _server;

    /**
     * Generate a unique lobby ID.
     *
     * Returns:
     *     uint32_t: New unique lobby ID
     */
    uint32_t generateLobbyId();

    /**
     * Main loop for a lobby's game thread.
     *
     * Args:
     *     lobby (Lobby*): Pointer to the lobby to run
     */
    void runLobbyThread(Lobby* lobby);
};
