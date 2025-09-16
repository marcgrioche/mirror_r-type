#pragma once

#include <asio.hpp>
#include <cstdint>
#include <functional>
#include <vector>

/**
 * UdpSocket wraps UDP socket operations using asio.
 *
 * Args:
 *     port (uint16_t): UDP port to bind the socket.
 */
class UdpSocket {
public:
    explicit UdpSocket(uint16_t port);

    /**
     * Start asynchronous receive operation.
     *
     * Args:
     *     handler (function): Callback for received data and sender info.
     */
    void asyncReceive(std::function<void(const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)> handler);

    /**
     * Send data to a recipient.
     *
     * Args:
     *     data (const std::vector<uint8_t>&): Data to send.
     *     recipient (const asio::ip::udp::endpoint&): Recipient endpoint.
     */
    void send(const std::vector<uint8_t>& data, const asio::ip::udp::endpoint& recipient);

    /**
     * Close the socket and release resources.
     */
    void close();

private:
    uint16_t _port;
    asio::io_context _ioContext;
    asio::ip::udp::socket _socket;
};
