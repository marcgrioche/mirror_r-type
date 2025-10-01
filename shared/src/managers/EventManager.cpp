#include "EventManager.hpp"
#include <chrono>

EventManager& EventManager::getInstance()
{
    static EventManager instance;
    return instance;
}

void EventManager::subscribe(EventType type, EventHandler handler)
{
    subscribers[type].push_back(handler);
}

void EventManager::emit(const GameEvent& event)
{
    eventQueue.push_back(event);
}

void EventManager::emitButtonClick(Entity buttonEntity, const std::string& actionId)
{
    auto now = std::chrono::high_resolution_clock::now();
    float timestamp = std::chrono::duration<float>(now.time_since_epoch()).count();

    GameEvent event {
        EventType::BUTTON_CLICK,
        buttonEntity,
        Entity { 0, 0 }, // pas de target
        actionId,
        timestamp
    };
    emit(event);
}

void EventManager::processEvents()
{
    for (const auto& event : eventQueue) {
        auto it = subscribers.find(event.type);
        if (it != subscribers.end()) {
            for (const auto& handler : it->second) {
                handler(event);
            }
        }
    }
}

void EventManager::clear()
{
    eventQueue.clear();
}