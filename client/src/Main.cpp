/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Simple UDP client to test the server
*/

#include "Message.hpp"
#include "UdpSocket.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#ifdef _WIN32
#include <windows.h>
static std::atomic<bool> running(true);

BOOL WINAPI consoleCtrlHandler(DWORD ctrlType)
{
    if (ctrlType == CTRL_C_EVENT) {
        running = false;
        return TRUE;
    }
    return FALSE;
}
#else
#include <csignal>
std::atomic<bool> running(true);

void signalHandler(int)
{
    running = false;
}
#endif

void handleMessage(const std::vector<uint8_t>& data)
{
    try {
        Message msg = Message::deserialize(data);
        std::cout << "Received message type: " << static_cast<int>(msg.getType()) << std::endl;

        switch (msg.getType()) {
        case MessageType::CONNECT_ACK:
            std::cout << "Connected to server!" << std::endl;
            break;
        case MessageType::PONG:
            std::cout << "Pong received" << std::endl;
            break;
        case MessageType::GAME_STATE:
            std::cout << "Game state update received" << std::endl;
            break;
        default:
            std::cout << "Unknown message type" << std::endl;
            break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error deserializing message: " << e.what() << std::endl;
    }
}

int main()
{
#ifdef _WIN32
    SetConsoleCtrlHandler(consoleCtrlHandler, TRUE);
#else
    signal(SIGINT, signalHandler);
#endif

    try {
        UdpSocket socket(0); // Let OS assign port
        const std::string server_ip = "127.0.0.1";
        const uint16_t server_port = 4242;

        // Send CONNECT message
        Message connectMsg(MessageType::CONNECT, 1, 0, 1);
        std::vector<uint8_t> connectData = connectMsg.serialize();
        socket.send(connectData, server_ip, server_port);
        std::cout << "Sent CONNECT message" << std::endl;

        uint16_t sequence = 2;

        while (running) {
            // Poll for incoming messages
            if (socket.pollForData(100)) { // Poll with 100ms timeout
                std::vector<uint8_t> data;
                std::string sender_ip;
                uint16_t sender_port;

                ssize_t received = socket.receive(data, sender_ip, sender_port);
                if (received > 0) {
                    handleMessage(data);
                }
            }

            // Send PING message every 2 seconds
            static auto last_ping = std::chrono::steady_clock::now();
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - last_ping).count() >= 2) {
                Message pingMsg(MessageType::PING, sequence++, 0, 1);
                std::vector<uint8_t> pingData = pingMsg.serialize();
                socket.send(pingData, server_ip, server_port);
                std::cout << "Sent PING message" << std::endl;
                last_ping = now;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Small sleep to prevent busy waiting
        }

        socket.close();
        std::cout << "Client stopped" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
