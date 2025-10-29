#include "RTypeServer.hpp"
#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

#include "Metrics.hpp"

int main()
{
    RTypeServer server(4242);
    Promotheus::Metrics::Init();

    server.start();
    server.stop();
    return 0;
}
