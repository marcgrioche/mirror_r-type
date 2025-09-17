#include "UdpSocket.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>

#ifdef _WIN32
static bool winsock_initialized = false;

static void initializeWinsock()
{
    if (!winsock_initialized) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            std::cerr << "WSAStartup failed: " << result << std::endl;
            exit(1);
        }
        winsock_initialized = true;
    }
}

static void cleanupWinsock()
{
    if (winsock_initialized) {
        WSACleanup();
        winsock_initialized = false;
    }
}
#endif

/**
 * UdpSocket implementation using standard sockets.
 */

UdpSocket::UdpSocket(uint16_t port)
    : _port(port)
    , _socket(-1)
    , _is_open(false)
{
#ifdef _WIN32
    initializeWinsock();
#endif

    _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (_socket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(_socket, (sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "Failed to bind socket to port " << port << std::endl;
#ifdef _WIN32
        closesocket(_socket);
#else
        ::close(_socket);
#endif
        _socket = -1;
        return;
    }

    _is_open = true;
}

UdpSocket::~UdpSocket()
{
    close();
#ifdef _WIN32
    cleanupWinsock();
#endif
}

ssize_t UdpSocket::receive(std::vector<uint8_t>& buffer, std::string& sender_ip, uint16_t& sender_port)
{
    if (!_is_open)
        return -1;

    buffer.resize(2048);
    sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);

    ssize_t received = recvfrom(_socket, (char*)buffer.data(), buffer.size(), 0,
        (sockaddr*)&sender_addr, &addr_len);

    if (received > 0) {
        buffer.resize(received);
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sender_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
        sender_ip = ip_str;
        sender_port = ntohs(sender_addr.sin_port);
    } else if (received == -1) {
        std::cerr << "Receive error: " << strerror(errno) << std::endl;
    }

    return received;
}

ssize_t UdpSocket::send(const std::vector<uint8_t>& data, const std::string& recipient_ip, uint16_t recipient_port)
{
    if (!_is_open)
        return -1;

    sockaddr_in recipient_addr;
    std::memset(&recipient_addr, 0, sizeof(recipient_addr));
    recipient_addr.sin_family = AF_INET;
    recipient_addr.sin_port = htons(recipient_port);

    if (inet_pton(AF_INET, recipient_ip.c_str(), &recipient_addr.sin_addr) != 1) {
        std::cerr << "Invalid IP address: " << recipient_ip << std::endl;
        return -1;
    }

    ssize_t sent = sendto(_socket, (const char*)data.data(), data.size(), 0,
        (sockaddr*)&recipient_addr, sizeof(recipient_addr));

    if (sent == -1) {
        std::cerr << "Send error: " << strerror(errno) << std::endl;
    }

    return sent;
}

bool UdpSocket::pollForData(int timeout_ms)
{
    if (!_is_open)
        return false;

#ifdef _WIN32
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(_socket, &readfds);

    timeval timeout;
    if (timeout_ms >= 0) {
        timeout.tv_sec = timeout_ms / 1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000;
    }

    int result = select(0, &readfds, nullptr, nullptr, timeout_ms >= 0 ? &timeout : nullptr);
    return result > 0;
#else
    pollfd pfd;
    pfd.fd = _socket;
    pfd.events = POLLIN;
    pfd.revents = 0;

    int result = poll(&pfd, 1, timeout_ms);
    return result > 0 && (pfd.revents & POLLIN);
#endif
}

void UdpSocket::close()
{
    if (_is_open) {
#ifdef _WIN32
        closesocket(_socket);
#else
        ::close(_socket);
#endif
        _socket = -1;
        _is_open = false;
    }
}
