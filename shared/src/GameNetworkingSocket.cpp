#include "GameNetworkingSocket.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>

static bool gns_initialized = false;
static GameNetworkingSocket* s_instance = nullptr;

static void initializeGNS()
{
    if (!gns_initialized) {
        SteamNetworkingErrMsg errMsg;
        if (!GameNetworkingSockets_Init(nullptr, errMsg)) {
            std::cerr << "GameNetworkingSockets_Init failed: " << errMsg << std::endl;
            exit(1);
        }
        gns_initialized = true;
    }
}

static void cleanupGNS()
{
    if (gns_initialized) {
        GameNetworkingSockets_Kill();
        gns_initialized = false;
    }
}

void GameNetworkingSocket::ConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    if (s_instance) {
        s_instance->OnConnectionStatusChanged(pInfo);
    }
}

void GameNetworkingSocket::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    switch (pInfo->m_info.m_eState) {
    case k_ESteamNetworkingConnectionState_Connecting:
        if (_mode == SocketMode::SERVER) {
            if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_None) {
                // New connection, accept
                if (_networking->AcceptConnection(pInfo->m_hConn) == k_EResultOK) {
                    std::cout << "[GNS] Accepted new client connection" << std::endl;
                    ClientConnection client { pInfo->m_hConn, pInfo->m_info.m_addrRemote };
                    _clients.push_back(client);
                } else {
                    std::cerr << "[GNS] Failed to accept connection" << std::endl;
                    _networking->CloseConnection(pInfo->m_hConn, k_ESteamNetConnectionEnd_App_Generic, "Failed to accept", false);
                }
            }
        }
        break;
    case k_ESteamNetworkingConnectionState_Connected:
        std::cout << "[GNS] Connection established" << std::endl;
        break;
    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        std::cout << "[GNS] Connection closed" << std::endl;
        // Remove from clients
        _clients.erase(std::remove_if(_clients.begin(), _clients.end(),
                           [pInfo](const ClientConnection& c) { return c.conn == pInfo->m_hConn; }),
            _clients.end());
        _networking->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
        break;
    default:
        break;
    }
}

/**
 * GameNetworkingSocket implementation using Valve's GameNetworkingSockets.
 */

GameNetworkingSocket::GameNetworkingSocket(uint16_t port, const std::string& server_ip, uint16_t server_port)
    : _mode(server_ip.empty() ? SocketMode::SERVER : SocketMode::CLIENT)
    , _port(port)
    , _server_ip(server_ip)
    , _server_port(server_port)
    , _listen_socket(k_HSteamListenSocket_Invalid)
    , _connection(k_HSteamNetConnection_Invalid)
    , _networking(nullptr)
    , _is_open(false)
{
    s_instance = this;
    initializeGNS();

    _networking = SteamNetworkingSockets();

    if (_mode == SocketMode::SERVER) {
        // Create listen socket
        SteamNetworkingIPAddr addr;
        addr.Clear();
        addr.m_port = port;

        SteamNetworkingConfigValue_t opt;
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)&ConnectionStatusChangedCallback);

        _listen_socket = _networking->CreateListenSocketIP(addr, 1, &opt);
        if (_listen_socket == k_HSteamListenSocket_Invalid) {
            std::cerr << "[GNS] Failed to create listen socket on port " << port << std::endl;
            return;
        }
        std::cout << "[GNS] Server listen socket created on port " << port << std::endl;
        _is_open = true;
    } else {
        // Client mode: create connection
        SteamNetworkingIPAddr addr;
        addr.Clear();
        addr.ParseString(server_ip.c_str());
        addr.m_port = server_port;

        SteamNetworkingConfigValue_t opt;
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)&ConnectionStatusChangedCallback);

        _connection = _networking->ConnectByIPAddress(addr, 1, &opt);
        if (_connection == k_HSteamNetConnection_Invalid) {
            std::cerr << "[GNS] Failed to create connection to " << server_ip << ":" << server_port << std::endl;
            return;
        }
        std::cout << "[GNS] Client attempting connection to " << server_ip << ":" << server_port << std::endl;
        _is_open = true;
    }
}

GameNetworkingSocket::~GameNetworkingSocket()
{
    close();
    cleanupGNS();
}

ssize_t GameNetworkingSocket::receive(std::vector<uint8_t>& buffer, std::string& sender_ip, uint16_t& sender_port)
{
    if (!_is_open)
        return -1;

    if (_mode == SocketMode::CLIENT) {
        ISteamNetworkingMessage* pIncomingMsg = nullptr;
        int numMsgs = _networking->ReceiveMessagesOnConnection(_connection, &pIncomingMsg, 1);
        if (numMsgs > 0) {
            buffer.assign(static_cast<unsigned char*>(pIncomingMsg->m_pData), static_cast<unsigned char*>(pIncomingMsg->m_pData) + pIncomingMsg->m_cbSize);
            std::cout << "[GNS] Received " << buffer.size() << " bytes from server" << std::endl;
            // For client, sender is server
            sender_ip = _server_ip;
            sender_port = _server_port;
            pIncomingMsg->Release();
            return buffer.size();
        } else if (numMsgs < 0) {
            std::cerr << "[GNS] Receive error" << std::endl;
            return -1;
        }
    } else {
        // Server mode: poll all client connections
        for (auto& client : _clients) {
            ISteamNetworkingMessage* pIncomingMsg = nullptr;
            int numMsgs = _networking->ReceiveMessagesOnConnection(client.conn, &pIncomingMsg, 1);
            if (numMsgs > 0) {
                buffer.assign(static_cast<unsigned char*>(pIncomingMsg->m_pData), static_cast<unsigned char*>(pIncomingMsg->m_pData) + pIncomingMsg->m_cbSize);
                std::cout << "[GNS] Received " << buffer.size() << " bytes from client" << std::endl;
                // Get sender from client addr
                char ip[64];
                client.addr.ToString(ip, sizeof(ip), false);
                sender_ip = ip;
                sender_port = client.addr.m_port;
                pIncomingMsg->Release();
                return buffer.size();
            } else if (numMsgs < 0) {
                std::cerr << "[GNS] Receive error on client connection" << std::endl;
                return -1;
            }
        }
    }
    return 0; // No data
}

ssize_t GameNetworkingSocket::send(const std::vector<uint8_t>& data, const std::string& recipient_ip, uint16_t recipient_port)
{
    if (!_is_open)
        return -1;

    if (_mode == SocketMode::CLIENT) {
        // Send on connection
        EResult res = _networking->SendMessageToConnection(_connection, data.data(), data.size(), k_nSteamNetworkingSend_Reliable, nullptr);
        if (res == k_EResultOK) {
            std::cout << "[GNS] Sent " << data.size() << " bytes to server" << std::endl;
            return data.size();
        } else {
            std::cerr << "[GNS] Send error: " << res << std::endl;
            return -1;
        }
    } else {
        // Server mode: find connection to recipient
        for (auto& client : _clients) {
            char ip[64];
            client.addr.ToString(ip, sizeof(ip), false);
            if (recipient_ip == ip && recipient_port == client.addr.m_port) {
                EResult res = _networking->SendMessageToConnection(client.conn, data.data(), data.size(), k_nSteamNetworkingSend_Reliable, nullptr);
                if (res == k_EResultOK) {
                    std::cout << "[GNS] Sent " << data.size() << " bytes to client " << recipient_ip << ":" << recipient_port << std::endl;
                    return data.size();
                } else {
                    std::cerr << "[GNS] Send error to client: " << res << std::endl;
                    return -1;
                }
            }
        }
        std::cerr << "[GNS] Client not found: " << recipient_ip << ":" << recipient_port << std::endl;
        return -1;
    }
}

bool GameNetworkingSocket::pollForData(int timeout_ms)
{
    if (!_is_open)
        return false;

    if (_mode == SocketMode::CLIENT) {
        ISteamNetworkingMessage* pIncomingMsg = nullptr;
        int numMsgs = _networking->ReceiveMessagesOnConnection(_connection, &pIncomingMsg, 1);
        if (numMsgs > 0) {
            pIncomingMsg->Release(); // Discard for poll
            return true;
        }
    } else {
        // Server mode: check all client connections
        for (auto& client : _clients) {
            ISteamNetworkingMessage* pIncomingMsg = nullptr;
            int numMsgs = _networking->ReceiveMessagesOnConnection(client.conn, &pIncomingMsg, 1);
            if (numMsgs > 0) {
                pIncomingMsg->Release(); // Discard for poll
                return true;
            }
        }
    }
    return false;
}

bool GameNetworkingSocket::pollForWrite(int timeout_ms)
{
    if (!_is_open)
        return false;

    // Simplified: assume always ready
    return true;
}

void GameNetworkingSocket::close()
{
    if (_is_open) {
        // Close all client connections
        for (auto& client : _clients) {
            _networking->CloseConnection(client.conn, 0, nullptr, false);
        }
        _clients.clear();

        if (_mode == SocketMode::SERVER) {
            if (_listen_socket != k_HSteamListenSocket_Invalid) {
                _networking->CloseListenSocket(_listen_socket);
                _listen_socket = k_HSteamListenSocket_Invalid;
            }
        } else {
            if (_connection != k_HSteamNetConnection_Invalid) {
                _networking->CloseConnection(_connection, 0, nullptr, false);
                _connection = k_HSteamNetConnection_Invalid;
            }
        }
        _is_open = false;
    }
}