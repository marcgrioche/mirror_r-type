#pragma once
#include "RTypeNetwork.hpp"
#include <atomic>
#include <cstdint>
#include <memory>
#include <queue>

namespace Client {
class RTypeClient : public RTypeNetwork {
public:
    explicit RTypeClient(uint16_t port);

    void start() override;
    void stop() override;
    void requestStop() override;

    static void handleSignal(int t_signal);

private:
    void registerHandlers() override;
    uint16_t m_port;
};
}
