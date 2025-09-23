/**
 * IP Address and Port Encoding/Decoding Utility
 *
 * This file implements a custom encoding scheme that converts IP addresses and ports
 * into compact base-26 string representations using letters A-Z. This is useful for
 * creating short, URL-safe identifiers for network endpoints.
 *
 * Encoding Process:
 * 1. Convert IP address (a.b.c.d) to 32-bit integer
 * 2. Combine with port number (16-bit) to create 48-bit value
 * 3. Convert to base-26 using A-Z characters
 *
 * Example: 192.168.1.1:8080 -> "ABCDEF"
 */

#include "IpEncoding.hpp"
#include <cstring>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

/**
 * Gets the local IP address of the machine.
 *
 * @return The local IP address as a string (e.g., "192.168.1.1")
 */
std::string getLocalIp()
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        return "127.0.0.1";
    }
    struct hostent* host = gethostbyname(hostname);
    if (host && host->h_addr_list[0]) {
        struct in_addr addr;
        memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
        return inet_ntoa(addr);
    }
    return "127.0.0.1";
}

/**
 * Encodes an IP address and port combination into a compact base-26 string.
 *
 * The encoding process:
 * 1. Parse IP address into four octets (a.b.c.d)
 * 2. Convert IP to 32-bit number: (a << 24) + (b << 16) + (c << 8) + d
 * 3. Combine with port: (ip_num << 16) + port (creates 48-bit value)
 * 4. Convert to base-26 string using A-Z characters
 *
 * @param ip The IP address in dotted decimal format (e.g., "192.168.1.1")
 * @param port The port number (0-65535)
 * @return A base-26 encoded string representing the IP:port combination
 */
std::string encodeIp(std::string ip, int port)
{
    int a, b, c, d;

    sscanf(ip.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d);

    long long ip_num = (a << 24) + (b << 16) + (c << 8) + d;

    long long big_number = (ip_num << 16) + port;

    std::string result = "";
    while (big_number > 0) {
        result = char('A' + (big_number % 26)) + result;
        big_number = big_number / 26;
    }

    return result.empty() ? "A" : result;
}

/**
 * Decodes a base-26 encoded string back into an IP address and port combination.
 *
 * The decoding process (reverse of encoding):
 * 1. Convert base-26 string back to 48-bit integer
 * 2. Extract port from lower 16 bits: big_number & 0xFFFF
 * 3. Extract IP from upper 32 bits: big_number >> 16
 * 4. Convert IP number back to dotted decimal format (a.b.c.d)
 *
 * @param code The base-26 encoded string to decode
 * @param ip Output parameter: will contain the decoded IP address
 * @param port Output parameter: will contain the decoded port number
 */
void decodeIp(std::string code, std::string& ip, int& port)
{
    long long big_number = 0;
    int a, b, c, d;

    for (char ch : code) {
        big_number = big_number * 26 + (ch - 'A');
    }

    port = big_number & 0xFFFF;

    long long ip_num = big_number >> 16;

    a = (ip_num >> 24) & 0xFF;
    b = (ip_num >> 16) & 0xFF;
    c = (ip_num >> 8) & 0xFF;
    d = ip_num & 0xFF;

    ip = std::to_string(a) + "." + std::to_string(b) + "." + std::to_string(c) + "." + std::to_string(d);
}
