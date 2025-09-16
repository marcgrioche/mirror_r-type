#include "UdpSocket.hpp"
#include <iostream>

/**
 * UdpSocket implementation using Asio for UDP networking.
 */

UdpSocket::UdpSocket(uint16_t port)
    : _port(port)
    , _ioContext()
    , _socket(_ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
{
}

void UdpSocket::asyncReceive(std::function<void(const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)> handler)
{
    auto buffer = std::make_shared<std::vector<uint8_t>>(2048);
    auto sender = std::make_shared<asio::ip::udp::endpoint>();

    _socket.async_receive_from(
        asio::buffer(*buffer), *sender,
        [this, buffer, sender, handler](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                buffer->resize(bytes_recvd);
                handler(*buffer, *sender);
            } else if (ec) {
                std::cerr << "Receive error: " << ec.message() << std::endl;
            }
            asyncReceive(handler);
        });
    std::thread([this]() { _ioContext.run(); }).detach();
}

void UdpSocket::send(const std::vector<uint8_t>& data, const asio::ip::udp::endpoint& recipient)
{
    _socket.async_send_to(
        asio::buffer(data), recipient,
        [](std::error_code ec, std::size_t /*bytes_sent*/) {
            if (ec) {
                std::cerr << "Send error: " << ec.message() << std::endl;
            }
        });
}

void UdpSocket::close()
{
    std::error_code ec;
    _socket.close(ec);
    if (ec) {
        std::cerr << "Close error: " << ec.message() << std::endl;
    }
    _ioContext.stop();
}
