#include "RTypeServer.hpp"

void RTypeServer::handleAuthRequest(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo;
    const uint8_t usernameLength = msg.readU8();
    std::string username = msg.readString(usernameLength);
    const uint8_t passwordLength = msg.readU8();
    std::string password = msg.readString(passwordLength);
    uint32_t playerId = msg.player_id;

    Message response(MessageType::AUTH_RESPONSE, msg.sequence_number, msg.player_id);

    if (password.empty() || username.empty()) {
        response.write(static_cast<uint32_t>(0));
        response.write("Username or password cannot be empty");
        sendToClient(playerId, response);
        return;
    }
    if (_db->isUserRegistered(username)) {
        auto id = _db->authenticate(username, password);
        if (id != std::nullopt) {
            playerId = id.value();
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
            playerId = newId.value();
            _clients.erase(static_cast<uint32_t>(msg.player_id));
            _clients[static_cast<uint32_t>(newId.value())] = peerInfo;
            _usernames[static_cast<uint32_t>(newId.value())] = username;
            response.write(static_cast<uint32_t>(newId.value()));
        } else {
            response.write(static_cast<uint32_t>(0));
            response.write("Database not available!");
        }
    }
    sendToClient(playerId, response);
}