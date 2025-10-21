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
        const uint16_t clientPort = args.getPort();

        std::cout << "Using client port: " << clientPort << std::endl;

        Game game(clientPort);

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
