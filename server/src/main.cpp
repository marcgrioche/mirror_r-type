#include "RTypeServer.hpp"
#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

std::atomic<bool> running(true);

void signalHandler(int)
{
    running = false;
    std::cout << "\nStopping server gracefully..." << std::endl;
}

int main()
{
    std::signal(SIGINT, signalHandler);
    RTypeServer server(4242);
    server.start();

    while (running) {
        // TODO: Add server tick, event handling, etc.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    server.stop();
    return 0;
}
