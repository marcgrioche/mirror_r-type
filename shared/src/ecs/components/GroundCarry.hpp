#pragma once

struct GroundCarry {
    float vx = 0.0f; // vitesse horizontale imposée par la plateforme sous le joueur
    float vy = 0.0f; // si un jour plateformes verticales
    bool onPlatform = false; // vrai seulement le tick où on est posé
};
