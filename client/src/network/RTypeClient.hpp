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

namespace Client {
class RTypeClient : public RTypeNetwork {
public:
    explicit RTypeClient(uint16_t t_port, NetworkEventQueue& t_eventsQueue);
    RTypeClient(const std::string& t_serverIpAddress, uint16_t t_serverPort, uint16_t t_port,
        NetworkEventQueue& t_eventsQueue);

    void start() override;
    void stop() override;
    void requestStop() override;

    // setters

    /*
     * This function sets the server ip address and server port to
     * establish connection between client and server
     **/
    void setServerInfo(const std::string& t_serverIpAddress, uint16_t t_serverPort);
    void setDebugMode(bool t_debugMode);

    static void handleSignal(int t_signal);

    // server communication functions
    void sendMessage(MessageType t_type);
    void sendMessage(MessageType t_type, uint32_t t_payload);
    void sendMessage(const Message& t_msg);
    void sendMessageImmediately(const Message& t_msg);
    void connectToServerRequest();
    void disconnectFromServerRequest();
    void createLobbyRequest();
    void joinLobbyRequest(uint32_t t_lobbyId);
    void startGameRequest();
    void lobbyStartRequest();
    void pingRequest();
    void handleInputs(const InputManager& t_inputs);
    void handleAllInputs(const InputManager& t_inputs);
    void sendCurrentInputState(const std::vector<std::pair<GameInput, bool>>& inputs);

    uint32_t getCurrentTick() const { return m_currentTick; }
    void incrementTick() { m_currentTick++; }
    uint32_t getPlayerId() const { return m_playerId; }

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

    // server responses handlers
    void handleConnectionAccepted(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleLobbyCreation(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleLobbyJoint(const Message& t_msg, PeerInfo& t_peerInfo);
    void handlePongReceipt(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleSpawnEntity(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleGameState(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleGameEndWin(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleGameEndLose(const Message& t_msg, PeerInfo& t_peerInfo);
};
}
