#include "RTypeNetwork.hpp"
#include <iostream>

RTypeNetwork::RTypeNetwork()
{
    // Socket will be initialized by subclasses
}

void RTypeNetwork::requestStop()
{
    _running = false;
}

void RTypeNetwork::sendToPeer(const PeerInfo& peerInfo, const Message& msg)
{
    queueMessage(msg, peerInfo);
}

void RTypeNetwork::sendTo(const std::string& ip, uint16_t port, const Message& msg)
{
    PeerInfo peerInfo = { 0, ip, port };
    queueMessage(msg, peerInfo);
}

void RTypeNetwork::handleReceive(const Message& msg, const std::string& sender_ip, uint16_t sender_port)
{
    PeerInfo peerInfo = createPeerInfo(msg, sender_ip, sender_port);

    auto it = _handlers.find(msg.getType());
    if (it != _handlers.end()) {
        it->second(msg, peerInfo);
    } else {
        std::cerr << "Unknown message type received: " << static_cast<int>(msg.getType()) << std::endl;
    }
}

void RTypeNetwork::processIncomingMessages()
{
    if (_socket && _socket->pollForData(100)) { // TODO: smart timeout
        std::vector<uint8_t> data;
        std::string sender_ip;
        uint16_t sender_port;

        ssize_t received = _socket->receive(data, sender_ip, sender_port);
        if (received > 0) {
            try {
                Message msg = Message::deserialize(data);
                handleReceive(msg, sender_ip, sender_port);
            } catch (const std::exception& e) {
                std::cerr << "Error deserializing message: " << e.what() << std::endl;
            }
        }
    }
}

PeerInfo RTypeNetwork::createPeerInfo(const Message& msg, const std::string& sender_ip, uint16_t sender_port)
{
    return PeerInfo { msg.player_id, sender_ip, sender_port };
}

void RTypeNetwork::queueMessage(const Message& msg, const PeerInfo& peerInfo)
{
    _outgoingQueue.push({ msg, peerInfo });
}

void RTypeNetwork::processOutgoingMessages()
{
    if (_socket && _socket->pollForWrite(0)) {
        while (!_outgoingQueue.empty()) {
            QueuedMessage qmsg = _outgoingQueue.front();
            _outgoingQueue.pop();

            std::vector<uint8_t> data = qmsg.msg.serialize();
            _socket->send(data, qmsg.peerInfo.ip_address, qmsg.peerInfo.port);
        }
    }
}
