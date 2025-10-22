#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#ifdef _MSC_VER
typedef ptrdiff_t ssize_t;
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using socket_t = SOCKET;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t = int;
#endif

/**
 * @brief UdpSocket wraps UDP socket operations using standard sockets
 * @param port UDP port to bind the socket
 */
class UdpSocket {
public:
    /**
     * @brief Constructs a UDP socket bound to the specified port
     * @param port UDP port to bind the socket
     */
    explicit UdpSocket(uint16_t port);

    /**
     * @brief Destroys the UDP socket and releases resources
     */
    ~UdpSocket();

    /**
     * @brief Receives data synchronously from any sender
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
    socket_t _socket;
    bool _is_open;
};
