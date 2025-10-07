/*
** ProjectileSystem.cpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/ecs/systems
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 1:13:29 PM 2025 jojo
** Last update Wed Sep 23 2:03:27 PM 2025 jojo
*/

#include "ProjectileSystem.hpp"

void projectileSystem(Registry& registry, float deltaTime)
{
    // Utilise le tag pour rester cohérent avec les collisions
    auto view = registry.view<ProjectileTag, Lifetime>();

    for (auto it = view.begin(); it != view.end(); ++it) {
        auto [tag, lifetime] = *it;
        lifetime.value -= deltaTime;
        // Ne pas tuer ici: cleanupEntities() gère la suppression propre et évite les invalidations
    }
}
