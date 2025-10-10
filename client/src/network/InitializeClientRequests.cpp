#include "RTypeClient.hpp"

using namespace Client;

void RTypeClient::createUsernameRequest(const std::string& t_username)
{
    Message msg(MessageType::SET_USERNAME, m_msgSequenceNumber, m_playerId, 1);

    msg.write(t_username);
    sendMessage(msg);
    std::cout << "Username sent" << std::endl;
}
