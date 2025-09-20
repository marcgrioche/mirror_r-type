#pragma once
#include "RTypeNetwork.hpp"
#include <atomic>
#include <cstdint>
#include <memory>
#include <queue>

namespace Client {
class RTypeClient : public RTypeNetwork {
public:
    explicit RTypeClient(uint16_t t_port);
    RTypeClient(const std::string& t_serverIpAddress, uint16_t t_serverPort, uint16_t t_port);

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
    void connectToServerRequest();
    void disconnectFromServerRequest();
    void createLobbyRequest();
    void joinLobbyRequest(uint32_t t_lobbyId);
    void startGameRequest();
    void lobbyStartRequest();

private:
    void registerHandlers() override;
    uint16_t m_port;
    PeerInfo m_serverInfo;
    bool m_debug;
    uint16_t m_msgSequenceNumber;
    uint32_t m_lobbyId = 0;

    // server responses handlers
    void handleConnectionAccepted(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleLobbyCreation(const Message& t_msg, PeerInfo& t_peerInfo);
    void handleLobbyJoint(const Message& t_msg, PeerInfo& t_peerInfo);
};
}
