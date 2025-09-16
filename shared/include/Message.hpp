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

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdint>
#include <string>
#include <vector>

enum class MessageType : uint8_t {
    // Client to Server Messages
    CONNECT = 1,
    INPUT = 2,
    PING = 3,
    DISCONNECT = 4,

    // Server to Client Messages
    CONNECT_ACK = 101,
    GAME_STATE = 102,
    PONG = 103
};

class Message {
public:
    uint16_t sequence_number;
    uint32_t player_id;
    uint8_t version;

    Message(MessageType type, uint16_t seq = 0, uint32_t pid = 0, uint8_t ver = 1);
    Message(MessageType type, const std::vector<uint8_t>& payload, uint16_t seq = 0, uint32_t pid = 0, uint8_t ver = 1);

    void write(uint8_t value);
    void write(uint16_t value);
    void write(uint32_t value);
    void write(uint64_t value);
    void write(float value);
    void write(const std::string& value);
    void write(const std::vector<uint8_t>& data);

    uint8_t readU8() const;
    uint16_t readU16() const;
    uint32_t readU32() const;
    uint64_t readU64() const;
    float readFloat() const;
    std::string readString(uint8_t length) const;
    std::vector<uint8_t> readBytes(size_t length) const;

    mutable size_t readPos { 0 };

    std::vector<uint8_t> serialize() const;
    static Message deserialize(const std::vector<uint8_t>& data);

    MessageType getType() const { return type; }
    const std::vector<uint8_t>& getPayload() const { return payload; }
    size_t getReadPosition() const { return readPos; }
    void resetReadPosition() { readPos = 0; }

private:
    MessageType type;
    std::vector<uint8_t> payload;

    static uint16_t htons(uint16_t value);
    static uint16_t ntohs(uint16_t value);
    static uint32_t htonl(uint32_t value);
    static uint32_t ntohl(uint32_t value);
    static uint64_t htonll(uint64_t value);
    static uint64_t ntohll(uint64_t value);
};

#endif // MESSAGE_HPP
