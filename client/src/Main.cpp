/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** main
*/

#include "ProgramArguments.hpp"
#include "game_handler/Game.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    try {
        const ProgramArguments args(argc, argv);
        const bool isLocalMode = args.isLocalMode();
        const uint16_t clientPort = args.getPort();

        if (isLocalMode)
            std::cout << "Starting in local mode" << std::endl;
        std::cout << "Using client port: " << clientPort << std::endl;

        Game game(isLocalMode, clientPort);

        if (!game.initialize()) {
            std::cerr << "Failed to initialize game!" << std::endl;
            return 1;
        }

        game.run();

        std::cout << "Game completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
