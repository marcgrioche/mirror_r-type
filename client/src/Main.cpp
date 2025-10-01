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

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--local") {
            isLocalMode = true;
            std::cout << "Starting in local mode" << std::endl;
        }
    }

    Game game(isLocalMode);

    if (!game.initialize()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }

    game.run();

    std::cout << "Game completed successfully!" << std::endl;
    return 0;
}
