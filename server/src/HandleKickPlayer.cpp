#include "RTypeServer.hpp"
#include <iostream>

void RTypeServer::removePlayerAndNotifyLobby(const Message& msg, const Lobby* t_lobby, const uint32_t playerId)
{
    if (_lobbyManager.removePlayer(playerId)) {
        for (auto id : t_lobby->players) {
            Message notification(MessageType::KICK_NOTICE);
            notification.write(t_lobby->id);
            notification.write(playerId);
            notification.write(msg.player_id);
            sendToClient(id, notification);
        }
    }
}

void RTypeServer::handleKickPlayer(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo;
    uint32_t lobbyId = msg.readU32();
    uint32_t playerId = msg.readU32();
    Message response(MessageType::KICK_ACK);
    response.sequence_number = msg.sequence_number;
    response.player_id = msg.player_id;
    const auto lobby = _lobbyManager.getLobby(lobbyId);

    response.write(lobby->id);
    if (!lobby || lobby->creatorId != msg.player_id) {
        response.write(static_cast<uint8_t>(0));
        sendToClient(msg.player_id, response);
        return;
    }
    auto it = std::find(lobby->players.begin(), lobby->players.end(), playerId);
    if (it == lobby->players.end()) {
        response.write(static_cast<uint8_t>(0));
        sendToClient(msg.player_id, response);
        return;
    }
    response.write(static_cast<uint8_t>(1));
    sendToClient(msg.player_id, response);
    removePlayerAndNotifyLobby(msg, lobby, playerId);
}
