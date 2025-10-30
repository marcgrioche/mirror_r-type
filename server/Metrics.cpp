#include <memory>
#include <prometheus/counter.h>
#include <prometheus/exposer.h>
#include <prometheus/gauge.h>
#include <prometheus/registry.h>

using namespace prometheus;

static auto registry = std::make_shared<Registry>();

// define a counter family and one counter instance
static auto& connections_family = BuildCounter()
                                      .Name("server_connections_total")
                                      .Help("Total accepted connections")
                                      .Register(*registry);
static auto& connections = connections_family.Add({ { "service", "r-type" } });
// define a gauge for active connections
static auto& active_family = BuildGauge()
                                 .Name("server_active_connections")
                                 .Help("Current active connections")
                                 .Register(*registry);
static auto& active_connections = active_family.Add({ { "service", "r-type" } });

// start the HTTP endpoint (call once at startup)
void start_metrics_http()
{
    static Exposer exposer { "0.0.0.0:8080" };
    exposer.RegisterCollectable(registry);
}

/* Example usage in your server accept loop (pseudocode)
start_metrics_http();
// setup listening socket ...
while (true) {
    int client_fd = accept(listen_fd, ...);
    if (client_fd >= 0) {
    connections.Increment();     // total accepted
    active_connections.Increment(); // increment active
    // spawn thread or handle client
    // when client closes:
    // active_connections.Decrement();
    }
}
*/
