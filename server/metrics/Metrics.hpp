#pragma once
#include <memory>
#include <prometheus/counter.h>
#include <prometheus/exposer.h>
#include <prometheus/gauge.h>
#include <prometheus/registry.h>

using namespace prometheus;

namespace Promotheus {
class Metrics {
public:
    static void Init(const std::string& t_address = "0.0.0.0:8080");

    static Gauge* _connectionsGauge;
    static Counter* _packetsReceivedCounter;
    static Counter* _packetsSentCounter;

    static void IncrementPacketsReceived(uint64_t t_count = 1);
    static void IncrementPacketsSent(uint64_t t_count = 1);
    static void IncrementConnections(uint64_t t_count = 1);
    static void DecrementConnections(uint64_t t_count = 1);

private:
    static std::shared_ptr<prometheus::Registry> _registry;
    static std::unique_ptr<Exposer> _exposer;
};
}
