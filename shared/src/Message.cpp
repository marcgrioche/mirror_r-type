/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Apr, 2025                                                     *
 * Title           - mirror_jetpack                                                     *
 * Description     -                                                                    *
 *     Message                                                                          *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Message.hpp"
#include <cstring>
#include <stdexcept>

Message::Message(MessageType type, uint16_t seq, uint32_t pid, uint8_t ver)
    : sequence_number(seq)
    , player_id(pid)
    , version(ver)
    , type(type)
{
}

Message::Message(MessageType type, const std::vector<uint8_t>& payload, uint16_t seq, uint32_t pid, uint8_t ver)
    : type(type)
    , payload(payload)
    , sequence_number(seq)
    , player_id(pid)
    , version(ver)
{
}

void Message::write(uint8_t value)
{
    payload.push_back(value);
}

void Message::write(uint16_t value)
{
    uint16_t netValue = htons(value);
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&netValue);
    payload.insert(payload.end(), bytes, bytes + sizeof(netValue));
}

void Message::write(uint32_t value)
{
    uint32_t netValue = htonl(value);
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&netValue);
    payload.insert(payload.end(), bytes, bytes + sizeof(netValue));
}

void Message::write(uint64_t value)
{
    uint64_t netValue = htonll(value);
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&netValue);
    payload.insert(payload.end(), bytes, bytes + sizeof(netValue));
}

void Message::write(float value)
{
    uint32_t tmp;
    std::memcpy(&tmp, &value, sizeof(float));
    write(tmp);
}

void Message::write(const std::string& value)
{
    if (value.length() > 255) {
        throw std::runtime_error("String too long for protocol");
    }
    write(static_cast<uint8_t>(value.length()));
    payload.insert(payload.end(), value.begin(), value.end());
}

void Message::write(const std::vector<uint8_t>& data)
{
    payload.insert(payload.end(), data.begin(), data.end());
}

uint8_t Message::readU8() const
{
    if (readPos >= payload.size()) {
        throw std::runtime_error("Read beyond message bounds");
    }
    return payload[readPos++];
}

uint16_t Message::readU16() const
{
    if (readPos + sizeof(uint16_t) > payload.size()) {
        throw std::runtime_error("Read beyond message bounds");
    }
    uint16_t value;
    std::memcpy(&value, &payload[readPos], sizeof(value));
    readPos += sizeof(value);
    return ntohs(value);
}

uint32_t Message::readU32() const
{
    if (readPos + sizeof(uint32_t) > payload.size()) {
        throw std::runtime_error("Read beyond message bounds");
    }
    uint32_t value;
    std::memcpy(&value, &payload[readPos], sizeof(value));
    readPos += sizeof(value);
    return ntohl(value);
}

uint64_t Message::readU64() const
{
    if (readPos + sizeof(uint64_t) > payload.size()) {
        throw std::runtime_error("Read beyond message bounds");
    }
    uint64_t value;
    std::memcpy(&value, &payload[readPos], sizeof(value));
    readPos += sizeof(value);
    return ntohll(value);
}

float Message::readFloat() const
{
    uint32_t tmp = readU32();
    float value;
    std::memcpy(&value, &tmp, sizeof(float));
    return value;
}

std::string Message::readString(uint8_t length) const
{
    if (readPos + length > payload.size()) {
        throw std::runtime_error("Read beyond message bounds");
    }
    std::string value(payload.begin() + readPos, payload.begin() + readPos + length);
    readPos += length;
    return value;
}

std::vector<uint8_t> Message::readBytes(size_t length) const
{
    if (readPos + length > payload.size()) {
        throw std::runtime_error("Read beyond message bounds");
    }
    std::vector<uint8_t> data(payload.begin() + readPos, payload.begin() + readPos + length);
    readPos += length;
    return data;
}

std::vector<uint8_t> Message::serialize() const
{
    std::vector<uint8_t> data;
    data.push_back(static_cast<uint8_t>(type));
    data.push_back(version);

    uint16_t seq = htons(sequence_number);
    const uint8_t* seqBytes = reinterpret_cast<const uint8_t*>(&seq);
    data.insert(data.end(), seqBytes, seqBytes + sizeof(seq));

    uint32_t pid = htonl(player_id);
    const uint8_t* pidBytes = reinterpret_cast<const uint8_t*>(&pid);
    data.insert(data.end(), pidBytes, pidBytes + sizeof(pid));

    uint16_t length = htons(static_cast<uint16_t>(payload.size()));
    const uint8_t* lengthBytes = reinterpret_cast<const uint8_t*>(&length);
    data.insert(data.end(), lengthBytes, lengthBytes + sizeof(length));

    data.insert(data.end(), payload.begin(), payload.end());
    return data;
}

Message Message::deserialize(const std::vector<uint8_t>& data)
{
    if (data.size() < 10) {
        throw std::runtime_error("Message too short");
    }

    MessageType type = static_cast<MessageType>(data[0]);
    uint8_t version = data[1];

    uint16_t seq;
    std::memcpy(&seq, &data[2], sizeof(seq));
    seq = ntohs(seq);

    uint32_t pid;
    std::memcpy(&pid, &data[4], sizeof(pid));
    pid = ntohl(pid);

    uint16_t length;
    std::memcpy(&length, &data[8], sizeof(length));
    length = ntohs(length);

    if (data.size() != static_cast<size_t>(length) + 10) {
        throw std::runtime_error("Invalid message length");
    }

    std::vector<uint8_t> payload(data.begin() + 10, data.end());
    return Message(type, payload, seq, pid, version);
}

uint16_t Message::htons(uint16_t value)
{
    return (value << 8) | (value >> 8);
}

uint16_t Message::ntohs(uint16_t value)
{
    return (value << 8) | (value >> 8);
}

uint32_t Message::htonl(uint32_t value)
{
    return ((value & 0xFF) << 24) | ((value & 0xFF00) << 8) | ((value & 0xFF0000) >> 8) | ((value & 0xFF000000) >> 24);
}

uint32_t Message::ntohl(uint32_t value)
{
    return ((value & 0xFF) << 24) | ((value & 0xFF00) << 8) | ((value & 0xFF0000) >> 8) | ((value & 0xFF000000) >> 24);
}

uint64_t Message::htonll(uint64_t value)
{
    return ((value & 0xFFULL) << 56) | ((value & 0xFF00ULL) << 40) | ((value & 0xFF0000ULL) << 24) | ((value & 0xFF000000ULL) << 8) | ((value & 0xFF00000000ULL) >> 8) | ((value & 0xFF0000000000ULL) >> 24) | ((value & 0xFF000000000000ULL) >> 40) | ((value & 0xFF00000000000000ULL) >> 56);
}

uint64_t Message::ntohll(uint64_t value)
{
    return ((value & 0xFFULL) << 56) | ((value & 0xFF00ULL) << 40) | ((value & 0xFF0000ULL) << 24) | ((value & 0xFF000000ULL) << 8) | ((value & 0xFF00000000ULL) >> 8) | ((value & 0xFF0000000000ULL) >> 24) | ((value & 0xFF000000000000ULL) >> 40) | ((value & 0xFF00000000000000ULL) >> 56);
}
