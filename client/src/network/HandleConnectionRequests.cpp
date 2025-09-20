#include "RTypeClient.hpp"
#include <iostream>

using namespace Client;

void RTypeClient::handleConnectionAccepted(const Message& t_msg, PeerInfo& t_peerInfo)
{
    (void)t_msg;
    std::cout << "Connection accepted" << std::endl;
    if (m_serverInfo.ip_address != t_peerInfo.ip_address || m_serverInfo.port != t_peerInfo.port) {
        m_serverInfo = t_peerInfo;
    }
}
