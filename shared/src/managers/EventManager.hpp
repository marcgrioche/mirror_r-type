#pragma once
#include "Entity.hpp"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Enumeration of base event types in the game
 */
enum class EventType {
    BUTTON_CLICK, /**< Button click event */
    COLLISION_DETECTED, /**< Collision detection event */
    PLAYER_DIED, /**< Player death event */
    LEVEL_COMPLETE, /**< Level completion event */
    CUSTOM_EVENT /**< Custom user-defined event */
};

/**
 * @brief Generic game event structure
 */
struct GameEvent {
    EventType type; /**< Type of the event */
    Entity source; /**< Entity that emitted the event */
    Entity target; /**< Optional target entity */
    std::string data; /**< Serialized data (JSON/custom format) */
    float timestamp; /**< When the event occurred */
};

/**
 * @brief Event handler function type (callback)
 */
using EventHandler = std::function<void(const GameEvent&)>;

/**
 * @brief Singleton manager for handling game events
 */
class EventManager {
public:
    /**
     * @brief Gets the singleton instance of EventManager
     * @return Reference to the EventManager instance
     */
    static EventManager& getInstance();

    /**
     * @brief Subscribes to a specific event type
     * @param type The event type to subscribe to
     * @param handler The callback function to handle the event
     */
    void subscribe(EventType type, EventHandler handler);

    /**
     * @brief Emits an event (adds it to the event queue)
     * @param event The event to emit
     */
    void emit(const GameEvent& event);

    /**
     * @brief Helper function to emit a button click event
     * @param buttonEntity The button entity that was clicked
     * @param actionId The action identifier associated with the button
     */
    void emitButtonClick(Entity buttonEntity, const std::string& actionId);

    /**
     * @brief Processes all pending events (call this every frame)
     */
    void processEvents();

    /**
     * @brief Clears the event queue (call at end of frame)
     */
    void clear();

private:
    EventManager() = default;

    std::vector<GameEvent> eventQueue; /**< Queue of pending events */
    std::unordered_map<EventType, std::vector<EventHandler>> subscribers; /**< Event subscribers */
};
