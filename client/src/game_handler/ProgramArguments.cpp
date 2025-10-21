#include "ProgramArguments.hpp"
#include <stdexcept>

ProgramArguments::ProgramArguments(const int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
        m_arguments.emplace_back(argv[i]);
}

uint16_t ProgramArguments::getPort() const
{
    for (size_t i = 0; i + 1 < m_arguments.size(); ++i) {
        if (m_arguments[i] == "--port") {
            const int port = std::atoi(m_arguments[i + 1].c_str());
            if (port <= 0 || port > 65535) {
                throw std::invalid_argument("Invalid port number");
            }
            return static_cast<uint16_t>(port);
        }
    }
    return 2020;
}