#include "RTypeClient.hpp"

using namespace Client;

void RTypeClient::handleKickPlayerAck(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;
    Message msg = t_msg;
    msg.resetReadPosition();
    const uint32_t lobbyId = msg.readU32();
    const uint32_t playerId = msg.readU32();
    const uint8_t status = msg.readU8();

    if (status) {
        std::cout << "Player " << playerId << " Kicked from lobby " << lobbyId << std::endl;
    } else {
        std::cout << "Player " << playerId << " Kick failed." << lobbyId << std::endl;
    }
}

void RTypeClient::handleKickPlayerNotice(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;
    Message msg = t_msg;
    msg.resetReadPosition();
    const uint32_t lobbyID = msg.readU32();

    std::cout << "Kicking a player from lobby " << lobbyID << std::endl;
    m_eventsQueue.push({ MessageType::KICK_NOTICE, msg });
}

void RTypeClient::kickPlayerFromLobby(const uint32_t t_playerId)
{
    Message request(MessageType::KICK_PLAYER, m_msgSequenceNumber, m_playerId);

    request.write(m_lobbyId);
    request.write(t_playerId);
    sendMessage(request);
}
