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
     * @brief Gets the port number from command line arguments
     * @return The port number
     */
    uint16_t getPort() const;

    /**
     * @brief Gets the colorblind filter type
     * @return The colorblind filter type ("protanopia", "deuteranopia", "tritanopia", or empty string for no filter)
     */
    std::string getColorblindType() const;

private:
    std::vector<std::string> m_arguments;
    std::string m_colorblindType;
};
