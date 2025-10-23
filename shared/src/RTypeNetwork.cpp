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
    int messagesProcessed = 0;
    const int MAX_MESSAGES_PER_CALL = 50;

    while (_socket && _socket->pollForData(0) && messagesProcessed < MAX_MESSAGES_PER_CALL) {
        std::vector<uint8_t> data;
        std::string sender_ip;
        uint16_t sender_port;

        ssize_t received = _socket->receive(data, sender_ip, sender_port);
        if (received > 0) {
            size_t offset = 0;

            while (offset < data.size() && messagesProcessed < MAX_MESSAGES_PER_CALL) {
                try {
                    size_t bytesConsumed = 0;
                    Message msg = Message::deserializeFromOffset(data, offset, bytesConsumed);
                    handleReceive(msg, sender_ip, sender_port);

                    offset += bytesConsumed;
                    messagesProcessed++;
                } catch (const std::exception& e) {
                    std::cerr << "Error deserializing message at offset " << offset << ": " << e.what() << std::endl;
                    break;
                }
            }
        } else {
            break;
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
        std::map<PeerInfo, std::vector<Message>> peerMessages;

        while (!_outgoingQueue.empty()) {
            QueuedMessage qmsg = _outgoingQueue.front();
            _outgoingQueue.pop();
            peerMessages[qmsg.peerInfo].push_back(qmsg.msg);
        }

        for (const auto& [peer, messages] : peerMessages) {
            const size_t MAX_UDP_SIZE = 1400;
            std::vector<uint8_t> batchBuffer;
            size_t currentBatchSize = 0;

            for (const auto& msg : messages) {
                std::vector<uint8_t> serializedMsg = msg.serialize();

                if (currentBatchSize + serializedMsg.size() > MAX_UDP_SIZE && !batchBuffer.empty()) {
                    _socket->send(batchBuffer, peer.ip_address, peer.port);

                    batchBuffer.clear();
                    currentBatchSize = 0;
                }

                batchBuffer.insert(batchBuffer.end(), serializedMsg.begin(), serializedMsg.end());
                currentBatchSize += serializedMsg.size();
            }

            if (!batchBuffer.empty()) {
                _socket->send(batchBuffer, peer.ip_address, peer.port);
            }
        }
    }
}
