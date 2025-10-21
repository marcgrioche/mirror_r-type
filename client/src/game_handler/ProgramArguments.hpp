#pragma once
#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Class for parsing and managing program command line arguments
 */
class ProgramArguments {
public:
    /**
     * @brief Constructs a ProgramArguments object from command line arguments
     * @param argc Number of command line arguments
     * @param argv Array of command line argument strings
     */
    ProgramArguments(int argc, char** argv);

    /**
     * @brief Default destructor
     */
    ~ProgramArguments() = default;

    /**
     * @brief Checks if the program is running in local mode
     * @return True if local mode is enabled, false otherwise
     */
    bool isLocalMode() const;

    /**
     * @brief Gets the port number from command line arguments
     * @return The port number
     */
    uint16_t getPort() const;

private:
    std::vector<std::string> m_arguments;
};
