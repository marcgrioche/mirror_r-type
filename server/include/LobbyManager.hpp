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
    std::unordered_map<uint32_t, std::string> _usernames;

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
     * @brief LobbyManager handles creation, management, and state tracking of game lobbies.
     * Thread-safe for concurrent access from multiple threads.
     */
    LobbyManager();

    /**
     * @brief Sets the server reference for broadcasting messages
     * @param server Pointer to the server instance
     */
    void setServer(RTypeServer* server);

    /**
     * @brief Creates a new lobby with the specified creator
     * @param creatorId Player ID of the lobby creator
     * @return Unique lobby ID, or 0 if creation failed
     */
    uint32_t createLobby(uint32_t creatorId);

    /**
     * @brief Joins an existing lobby
     * @param lobbyId ID of the lobby to join
     * @param playerId Player ID wanting to join
     * @return True if join was successful, false otherwise
     */
    bool joinLobby(uint32_t lobbyId, uint32_t playerId);

    /**
     * @brief Starts a game in the specified lobby (only lobby creator can do this)
     * @param lobbyId ID of the lobby to start
     * @param playerId Player ID attempting to start (must be creator)
     * @return True if game start was successful, false otherwise
     */
    bool startGame(uint32_t lobbyId, uint32_t playerId);

    /**
     * @brief Gets lobby information
     * @param lobbyId ID of the lobby to query
     * @return Pointer to lobby info, or nullptr if not found
     */
    const Lobby* getLobby(uint32_t lobbyId) const;

    /**
     * @brief Gets the lobby ID that a player is currently in
     * @param playerId Player ID to check
     * @return Lobby ID, or 0 if player is not in any lobby
     */
    uint32_t getPlayerLobby(uint32_t playerId) const;

    /**
     * @brief Removes a player from their current lobby
     * @param playerId Player ID to remove
     * @return True if player was removed, false if not in any lobby
     */
    bool removePlayer(uint32_t playerId);

    /**
     * @brief Gets all active lobbies
     * @return List of active lobby IDs
     */
    std::vector<uint32_t> getActiveLobbies() const;

    /**
     * @brief Gets the GameInstance for a lobby
     * @param lobbyId ID of the lobby
     * @return Pointer to the game instance, or nullptr if not running
     */
    GameInstance* getGameInstance(uint32_t lobbyId);

    /**
     * @brief Gets the players in a lobby (thread-safe)
     * @param lobbyId ID of the lobby
     * @return List of player IDs in the lobby
     */
    std::vector<uint32_t> getLobbyPlayers(uint32_t lobbyId) const;

    /**
     * @brief Adds a username for a player
     * @param playerId Player ID to set username for
     * @param username The username to set
     * @return True if username was added successfully, false otherwise
     */
    bool addUsername(uint32_t playerId, const std::string& username);

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
