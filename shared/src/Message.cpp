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
    : sequence_number(seq)
    , player_id(pid)
    , version(ver)
    , type(type)
    , payload(payload)
{
}

Message::Message(const Message& other)
    : sequence_number(other.sequence_number)
    , player_id(other.player_id)
    , version(other.version)
    , readPos(0)
    , type(other.type)
    , payload(other.payload)
{
}

void Message::write(uint8_t value)
{
    payload.push_back(value);
}

void Message::write(uint16_t value)
{
    uint16_t netValue = byteSwap16(value);
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&netValue);
    payload.insert(payload.end(), bytes, bytes + sizeof(netValue));
}

void Message::write(uint32_t value)
{
    uint32_t netValue = byteSwap32(value);
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&netValue);
    payload.insert(payload.end(), bytes, bytes + sizeof(netValue));
}

void Message::write(uint64_t value)
{
    uint64_t netValue = byteSwap64(value);
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
    if (!canRead(sizeof(uint8_t))) {
        throw std::runtime_error("Read beyond message bounds: trying to read uint8_t, " + std::to_string(remainingBytes()) + " bytes remaining");
    }
    return payload[readPos++];
}

uint16_t Message::readU16() const
{
    if (!canRead(sizeof(uint16_t))) {
        throw std::runtime_error("Read beyond message bounds: trying to read uint16_t, " + std::to_string(remainingBytes()) + " bytes remaining");
    }
    uint16_t value;
    std::memcpy(&value, &payload[readPos], sizeof(value));
    readPos += sizeof(value);
    return byteSwap16(value);
}

uint32_t Message::readU32() const
{
    if (!canRead(sizeof(uint32_t))) {
        throw std::runtime_error("Read beyond message bounds: trying to read uint32_t, " + std::to_string(remainingBytes()) + " bytes remaining");
    }
    uint32_t value;
    std::memcpy(&value, &payload[readPos], sizeof(value));
    readPos += sizeof(value);
    return byteSwap32(value);
}

uint64_t Message::readU64() const
{
    if (!canRead(sizeof(uint64_t))) {
        throw std::runtime_error("Read beyond message bounds: trying to read uint64_t, " + std::to_string(remainingBytes()) + " bytes remaining");
    }
    uint64_t value;
    std::memcpy(&value, &payload[readPos], sizeof(value));
    readPos += sizeof(value);
    return byteSwap64(value);
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
    if (!canRead(length)) {
        throw std::runtime_error("Read beyond message bounds: trying to read string of length " + std::to_string(length) + ", " + std::to_string(remainingBytes()) + " bytes remaining");
    }
    std::string value(payload.begin() + readPos, payload.begin() + readPos + length);
    readPos += length;
    return value;
}

std::vector<uint8_t> Message::readBytes(size_t length) const
{
    if (!canRead(length)) {
        throw std::runtime_error("Read beyond message bounds: trying to read " + std::to_string(length) + " bytes, " + std::to_string(remainingBytes()) + " bytes remaining");
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

    uint16_t seq = byteSwap16(sequence_number);
    const uint8_t* seqBytes = reinterpret_cast<const uint8_t*>(&seq);
    data.insert(data.end(), seqBytes, seqBytes + sizeof(seq));

    uint32_t pid = byteSwap32(player_id);
    const uint8_t* pidBytes = reinterpret_cast<const uint8_t*>(&pid);
    data.insert(data.end(), pidBytes, pidBytes + sizeof(pid));

    uint16_t length = byteSwap16(static_cast<uint16_t>(payload.size()));
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
    seq = byteSwap16(seq);

    uint32_t pid;
    std::memcpy(&pid, &data[4], sizeof(pid));
    pid = byteSwap32(pid);

    uint16_t length;
    std::memcpy(&length, &data[8], sizeof(length));
    length = byteSwap16(length);

    if (data.size() != static_cast<size_t>(length) + 10) {
        throw std::runtime_error("Invalid message length");
    }

    std::vector<uint8_t> payload(data.begin() + 10, data.end());
    return Message(type, payload, seq, pid, version);
}

uint16_t Message::byteSwap16(uint16_t value)
{
    return (value << 8) | (value >> 8);
}

uint16_t Message::byteSwap16Reverse(uint16_t value)
{
    return byteSwap16(value);
}

uint32_t Message::byteSwap32(uint32_t value)
{
    return ((value & 0xFF) << 24) | ((value & 0xFF00) << 8) | ((value & 0xFF0000) >> 8) | ((value & 0xFF000000) >> 24);
}

uint32_t Message::byteSwap32Reverse(uint32_t value)
{
    return byteSwap32(value);
}

uint64_t Message::byteSwap64(uint64_t value)
{
    return ((value & 0xFFULL) << 56) | ((value & 0xFF00ULL) << 40) | ((value & 0xFF0000ULL) << 24) | ((value & 0xFF000000ULL) << 8) | ((value & 0xFF00000000ULL) >> 8) | ((value & 0xFF0000000000ULL) >> 24) | ((value & 0xFF000000000000ULL) >> 40) | ((value & 0xFF00000000000000ULL) >> 56);
}

uint64_t Message::byteSwap64Reverse(uint64_t value)
{
    return byteSwap64(value);
}

std::ostream& operator<<(std::ostream& t_os, const MessageType t_type)
{
    t_os << (static_cast<int>(t_type));
    return t_os;
}
