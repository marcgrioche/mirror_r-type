#pragma once
#include <cstdint>
#include <string>
#include <vector>

class ProgramArguments {
public:
    ProgramArguments(int argc, char** argv);
    ~ProgramArguments() = default;
    bool isLocalMode() const;
    uint16_t getPort() const;

private:
    std::vector<std::string> m_arguments;
    int m_argc;
};
