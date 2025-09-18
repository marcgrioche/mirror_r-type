#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

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
 * UdpSocket wraps UDP socket operations using standard sockets.
 *
 * Args:
 *     port (uint16_t): UDP port to bind the socket.
 */
class UdpSocket {
public:
    explicit UdpSocket(uint16_t port);
    ~UdpSocket();

    /**
     * Receive data synchronously.
     *
     * Args:
     *     buffer (std::vector<uint8_t>&): Buffer to store received data.
     *     sender_ip (std::string&): Sender IP address.
     *     sender_port (uint16_t&): Sender port.
     *
     * Returns:
     *     ssize_t: Number of bytes received, or -1 on error.
     */
    ssize_t receive(std::vector<uint8_t>& buffer, std::string& sender_ip, uint16_t& sender_port);

    /**
     * Send data to a recipient.
     *
     * Args:
     *     data (const std::vector<uint8_t>&): Data to send.
     *     recipient_ip (const std::string&): Recipient IP address.
     *     recipient_port (uint16_t): Recipient port.
     *
     * Returns:
     *     ssize_t: Number of bytes sent, or -1 on error.
     */
    ssize_t send(const std::vector<uint8_t>& data, const std::string& recipient_ip, uint16_t recipient_port);

    /**
     * Poll for incoming data with timeout.
     *
     * Args:
     *     timeout_ms (int): Timeout in milliseconds (-1 for infinite).
     *
     * Returns:
     *     bool: True if data is available, false otherwise.
     */
    bool pollForData(int timeout_ms = -1);

    /**
     * Poll for write readiness with timeout.
     *
     * Args:
     *     timeout_ms (int): Timeout in milliseconds (-1 for infinite).
     *
     * Returns:
     *     bool: True if socket is ready for writing, false otherwise.
     */
    bool pollForWrite(int timeout_ms = -1);

    /**
     * Close the socket and release resources.
     */
    void close();

private:
    uint16_t _port;
    socket_t _socket;
    bool _is_open;
};
