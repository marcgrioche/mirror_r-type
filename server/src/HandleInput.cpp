#include "RTypeServer.hpp"
#include <iostream>

/**
 * Handle INPUT message from a client.
 *
 * Args:
 *     msg (const Message&): The received message.
 *     peerInfo (PeerInfo&): Information about the client.
 */
void RTypeServer::handleInput(const Message& msg, PeerInfo& peerInfo)
{
    (void)peerInfo; // Unused parameter

    uint32_t playerLobby = _lobbyManager.getPlayerLobby(msg.player_id);
    if (playerLobby == 0) {
        std::cout << "Player " << msg.player_id << " sent input but is not in any lobby" << std::endl;
        return;
    }

    const Lobby* lobby = _lobbyManager.getLobby(playerLobby);
    if (!lobby || lobby->state != LobbyState::RUNNING) {
        return;
    }

    if (msg.getPayload().size() < 5) {
        std::cout << "Invalid INPUT message payload size: " << msg.getPayload().size() << std::endl;
        return;
    }

    uint32_t clientTick = msg.readU32();
    uint8_t numInputs = msg.readU8();

    std::vector<std::pair<GameInput, bool>> inputs;

    for (uint8_t i = 0; i < numInputs; ++i) {
        if (msg.getReadPosition() + 2 > msg.getPayload().size()) {
            std::cout << "INPUT message truncated, expected more input data" << std::endl;
            break;
        }

        uint8_t inputTypeRaw = msg.readU8();
        uint8_t inputState = msg.readU8();

        GameInput inputType = static_cast<GameInput>(inputTypeRaw);
        bool isPressed = (inputState != 0);
        inputs.emplace_back(inputType, isPressed);
    }

    float mouseX = 0.0f;
    float mouseY = 0.0f;
    if (msg.remainingBytes() >= 8) {  // 2 floats = 8 bytes
        mouseX = msg.readFloat();
        mouseY = msg.readFloat();
    }

    PlayerInput playerInput { msg.player_id, clientTick, inputs, mouseX, mouseY };
    const_cast<Lobby*>(lobby)->queueInput(playerInput);
}
