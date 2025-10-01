#include "RTypeClient.hpp"

using namespace Client;

void RTypeClient::handleSpawnEntity(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_peerInfo;

    // Push the message to the event queue for Game to process
    m_eventsQueue.push({ MessageType::SPAWN_ENTITY, t_msg });
}
