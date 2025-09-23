/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** main
*/

#include "game_handler/Game.hpp"
#include <iostream>

int main()
{
    Game game;

    if (!game.initialize()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }

    game.run();

    std::cout << "Game completed successfully!" << std::endl;
    return 0;
}
