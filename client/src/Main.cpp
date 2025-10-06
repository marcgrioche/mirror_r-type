/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** main
*/

#include "game_handler/Game.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    bool isLocalMode = false;
    uint16_t clientPort = 2020;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--local") {
            isLocalMode = true;
            std::cout << "Starting in local mode" << std::endl;
        } else if (arg == "--port" && i + 1 < argc) {
            try {
                clientPort = static_cast<uint16_t>(std::stoi(argv[++i]));
                std::cout << "Using client port: " << clientPort << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Invalid port number: " << argv[i] << std::endl;
                return 1;
            }
        }
    }

    Game game(isLocalMode, clientPort);

    if (!game.initialize()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }

    game.run();

    std::cout << "Game completed successfully!" << std::endl;
    return 0;
}
