#pragma once

#include <mutex>
#include <optional>
#include <queue>
#include <variant>

#include "Message.hpp"

using NetworkPayload = std::variant<bool, std::string, Message>;

namespace Client {

/**
 * @brief Structure representing a network event with type and payload
 * @param type The type of the network message
 * @param payload The payload data associated with the event
 */
struct NetworkEvent {
    MessageType type;
    NetworkPayload payload;
};

/**
 * @brief Thread-safe queue for managing network events
 */
class NetworkEventQueue {
    std::queue<NetworkEvent> m_queue;
    std::mutex m_mutex;

public:
    /**
     * @brief Adds a network event to the queue
     * @param t_event The network event to add
     */
    void push(const NetworkEvent& t_event)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(t_event);
    }

    /**
     * @brief Removes and returns the next network event from the queue
     * @return An optional containing the event if available, nullopt if queue is empty
     */
    std::optional<NetworkEvent> pop()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty())
            return std::nullopt;
        auto event = m_queue.front();
        m_queue.pop();
        return event;
    }
};
}
