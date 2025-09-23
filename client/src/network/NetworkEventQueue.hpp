#pragma once

#include <mutex>
#include <optional>
#include <queue>
#include <variant>

#include "Message.hpp"

using NetworkPayload = std::variant<bool, std::string>;

namespace Client {
struct NetworkEvent {
    MessageType type;
    NetworkPayload payload;
};

class NetworkEventQueue {
    std::queue<NetworkEvent> m_queue;
    std::mutex m_mutex;

public:
    void push(const NetworkEvent& t_event)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(t_event);
    }

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
