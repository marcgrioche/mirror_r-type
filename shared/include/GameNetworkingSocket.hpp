#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#ifdef _MSC_VER
typedef ptrdiff_t ssize_t;
#endif

// Include GameNetworkingSockets headers
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingsockets.h>

enum class SocketMode {
    SERVER,
    CLIENT
};

/**
 * @brief GameNetworkingSocket wraps Valve's GameNetworkingSockets API
 * @param port Local port to bind (for server) or local port for client
 * @param server_ip Server IP for client mode (empty for server mode)
 * @param server_port Server port for client mode
 */
class GameNetworkingSocket {
public:
    /**
     * @brief Constructs a GameNetworkingSocket
     * @param port Local port to bind
     * @param server_ip Server IP (empty string for server mode)
     * @param server_port Server port (0 for server mode)
     */
    explicit GameNetworkingSocket(uint16_t port, const std::string& server_ip = "", uint16_t server_port = 0);

    /**
     * @brief Destroys the socket and releases resources
     */
    ~GameNetworkingSocket();

    /**
     * @brief Receives data synchronously
     * @param buffer Buffer to store received data
     * @param sender_ip Output parameter: sender IP address
     * @param sender_port Output parameter: sender port
     * @return Number of bytes received, or -1 on error
     */
    ssize_t receive(std::vector<uint8_t>& buffer, std::string& sender_ip, uint16_t& sender_port);

    /**
     * @brief Sends data to a specific recipient
     * @param data Data to send
     * @param recipient_ip Recipient IP address
     * @param recipient_port Recipient port
     * @return Number of bytes sent, or -1 on error
     */
    ssize_t send(const std::vector<uint8_t>& data, const std::string& recipient_ip, uint16_t recipient_port);

    /**
     * @brief Polls for incoming data with timeout
     * @param timeout_ms Timeout in milliseconds (-1 for infinite wait)
     * @return True if data is available, false otherwise
     */
    bool pollForData(int timeout_ms = -1);

    /**
     * @brief Polls for write readiness with timeout
     * @param timeout_ms Timeout in milliseconds (-1 for infinite wait)
     * @return True if socket is ready for writing, false otherwise
     */
    bool pollForWrite(int timeout_ms = -1);

    /**
     * @brief Closes the socket and releases resources
     */
    void close();

private:
    struct ClientConnection {
        HSteamNetConnection conn;
        SteamNetworkingIPAddr addr;
    };

    SocketMode _mode;
    uint16_t _port;
    std::string _server_ip;
    uint16_t _server_port;
    HSteamListenSocket _listen_socket;
    HSteamNetConnection _connection;
    ISteamNetworkingSockets* _networking;
    std::vector<ClientConnection> _clients;
    bool _is_open;

    static void ConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);
    void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);
};