#pragma once

#include <string>

/**
 * Gets the local IP address of the machine.
 *
 * @return The local IP address as a string (e.g., "192.168.1.1")
 */
std::string getLocalIp();

/**
 * Encodes an IP address and port combination into a compact base-26 string.
 *
 * @param ip The IP address in dotted decimal format (e.g., "192.168.1.1")
 * @param port The port number (0-65535)
 * @return A base-26 encoded string representing the IP:port combination
 */
std::string encodeIp(std::string ip, int port);

/**
 * Decodes a base-26 encoded string back into an IP address and port combination.
 *
 * @param code The base-26 encoded string to decode
 * @param ip Output parameter: will contain the decoded IP address
 * @param port Output parameter: will contain the decoded port number
 */
void decodeIp(std::string code, std::string& ip, int& port);
