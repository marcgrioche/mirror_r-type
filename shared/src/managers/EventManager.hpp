#pragma once
#include "Entity.hpp"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// Types d'événements de base
enum class EventType {
    BUTTON_CLICK,
    COLLISION_DETECTED,
    PLAYER_DIED,
    LEVEL_COMPLETE,
    CUSTOM_EVENT
};

// Structure d'événement générique
struct GameEvent {
    EventType type;
    Entity source; // qui a émis l'événement
    Entity target; // cible optionnelle
    std::string data; // données sérialisées (JSON/custom)
    float timestamp; // quand l'événement s'est produit
};

// Handler = fonction callback
using EventHandler = std::function<void(const GameEvent&)>;

class EventManager {
public:
    static EventManager& getInstance();

    // S'abonner à un type d'événement
    void subscribe(EventType type, EventHandler handler);

    // Émettre un événement (ajouté à la queue)
    void emit(const GameEvent& event);

    // Helper pour boutons
    void emitButtonClick(Entity buttonEntity, const std::string& actionId);

    // Traiter tous les événements en attente (à appeler chaque frame)
    void processEvents();

    // Vider la queue (fin de frame)
    void clear();

private:
    EventManager() = default;

    std::vector<GameEvent> eventQueue;
    std::unordered_map<EventType, std::vector<EventHandler>> subscribers;
};