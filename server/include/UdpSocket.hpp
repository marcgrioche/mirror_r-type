#pragma once

#include <cstdint>
#include <functional>
#include <vector>

// TODO : Forward declaration for ASIO types (replace with actual includes in implementation)
namespace asio {
namespace ip {
    class udp;
}
}

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
    void asyncReceive(std::function<void(const std::vector<uint8_t>& /*data*/)> handler);

    /**
     * Send data to a recipient.
     *
     * Args:
     *     data (const std::vector<uint8_t>&): Data to send.
     *     recipient (/* endpoint type ): Recipient endpoint.
     */
    void send(const std::vector<uint8_t>& data);

    /**
     * Close the socket and release resources.
     */
    void close();

private:
    uint16_t port_;
    // TODO : Add socket member here (e.g., asio::ip::udp::socket)
};
