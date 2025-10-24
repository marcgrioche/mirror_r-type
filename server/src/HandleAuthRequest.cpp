#include "RTypeServer.hpp"

void RTypeServer::handleAuthRequest(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo;
    const uint8_t usernameLength = msg.readU8();
    std::string username = msg.readString(usernameLength);
    const uint8_t passwordLength = msg.readU8();
    std::string password = msg.readString(passwordLength);

    Message response(MessageType::AUTH_RESPONSE, msg.sequence_number, msg.player_id);

    std::cout << "[SERVER] Authenticating " << username << " " << password << std::endl;
    if (_db->isUserRegistered(username)) {
        auto id = _db->authenticate(username, password);
        if (id != std::nullopt) {
            _clients.erase(static_cast<uint32_t>(msg.player_id));
            _clients[static_cast<uint32_t>(id.value())] = peerInfo;
            _usernames[static_cast<uint32_t>(id.value())] = username;
            response.write(static_cast<uint32_t>(id.value()));
        } else {
            response.write(static_cast<uint32_t>(0));
            response.write("Password is incorrect");
        }
    } else {
        auto newId = _db->registerPlayer(username, password);
        if (newId != std::nullopt) {
            _clients.erase(static_cast<uint32_t>(msg.player_id));
            _clients[static_cast<uint32_t>(newId.value())] = peerInfo;
            _usernames[static_cast<uint32_t>(newId.value())] = username;
            response.write(static_cast<uint32_t>(newId.value()));
        } else {
            response.write(static_cast<uint32_t>(0));
            response.write("Database not available!");
        }
    }
    sendToClient(response.player_id, response);
}