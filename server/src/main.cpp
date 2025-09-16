#include "RTypeServer.hpp"
#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

int main()
{
    RTypeServer server(4242);

    server.start();
    server.stop();
    return 0;
}
