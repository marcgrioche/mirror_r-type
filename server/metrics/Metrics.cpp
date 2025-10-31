#include "Metrics.hpp"

namespace Promotheus {
std::shared_ptr<prometheus::Registry> Metrics::_registry = nullptr;
std::unique_ptr<Exposer> Metrics::_exposer = nullptr;
Counter* Metrics::_packetsReceivedCounter = nullptr;
Counter* Metrics::_packetsSentCounter = nullptr;
Gauge* Metrics::_connectionsGauge = nullptr;

void Metrics::Init(const std::string& t_address)
{
    _exposer = std::make_unique<Exposer>(t_address);
    _registry = std::make_shared<Registry>();

    _exposer->RegisterCollectable(_registry);
    // Register a counter family on the shared registry and keep a pointer
    static auto& connections_family = BuildGauge()
                                          .Name("server_connections_total")
                                          .Help("Total active connections")
                                          .Register(*_registry);

    static auto& packets_family = BuildCounter()
                                      .Name("server_packets_total")
                                      .Help("Total received and sent packets")
                                      .Register(*_registry);

    _packetsReceivedCounter = &packets_family.Add({ { "type", "received" } });
    _packetsSentCounter = &packets_family.Add({ { "type", "sent" } });

    // Add an instance of the counter and store its address
    _connectionsGauge = &connections_family.Add({});
}

void Metrics::IncrementPacketsReceived(const uint64_t t_count)
{
    if (_packetsReceivedCounter != nullptr) {
        _packetsReceivedCounter->Increment(static_cast<double>(t_count));
    }
}

void Metrics::IncrementPacketsSent(const uint64_t t_count)
{
    if (_packetsSentCounter != nullptr) {
        _packetsSentCounter->Increment(static_cast<double>(t_count));
    }
}

void Metrics::IncrementConnections(const uint64_t t_count)
{
    if (_connectionsGauge != nullptr) {
        _connectionsGauge->Increment(static_cast<double>(t_count));
    }
}

void Metrics::DecrementConnections(const uint64_t t_count)
{
    if (_connectionsGauge != nullptr) {
        _connectionsGauge->Decrement(static_cast<double>(t_count));
    }
}

} // namespace Promotheus