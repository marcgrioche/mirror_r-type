/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** main
*/

#include "Components.hpp"
#include "Registry.hpp"
#include <iostream>

int main()
{
    Registry registry;

    // Crée deux entités
    Entity e1 = registry.spawn_entity();
    Entity e2 = registry.spawn_entity();

    // Ajoute des composants
    registry.add_component<Position>(e1, Position { 0, 0 });
    registry.add_component<Velocity>(e1, Velocity { 1, 1 });

    registry.add_component<Position>(e2, Position { 10, 5 });

    // Accès aux composants
    auto& positions = registry.get_components<Position>();
    auto& velocities = registry.get_components<Velocity>();

    for (size_t i = 0; i < positions.size(); i++) {
        if (positions[i]) {
            std::cout << "Entity " << i << " has Position("
                      << positions[i]->x << ", " << positions[i]->y << ")\n";
        }
    }

    for (size_t i = 0; i < velocities.size(); i++) {
        if (velocities[i]) {
            std::cout << "Entity " << i << " has Velocity("
                      << velocities[i]->dx << ", " << velocities[i]->dy << ")\n";
        }
    }

    return 0;
}
