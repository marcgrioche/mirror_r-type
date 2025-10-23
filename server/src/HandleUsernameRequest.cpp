#include "../include/RTypeServer.hpp"

void RTypeServer::handleUsername(const Message& msg, PeerInfo& peerInfo)
{
    // Extract the length of the username string
    (void)peerInfo;
    uint8_t usernameLength = msg.readU8();

    // Extract the actual username
    std::string username = msg.readString(usernameLength);

    // For now, just print (or log) the username
    std::cout << "Received username: " << username << std::endl;
    Message response(MessageType::USERNAME_ACK, msg.sequence_number, msg.player_id);
    const bool state = _lobbyManager.addUsername(msg.player_id, username);
    if (state) {
        response.write(static_cast<uint8_t>(1));
    } else {
        response.write(static_cast<uint8_t>(0));
    }
    // response.write(msg.player_id);
    // response.write(username);
    sendToClient(response.player_id, response);
    if (state) {
        const auto lobbyId = _lobbyManager.getPlayerLobby(msg.player_id);
        Message lobbyInfo = _lobbyManager.initLobbyInfo(lobbyId);
        broadcastToLobby(lobbyId, lobbyInfo);
    }
}
