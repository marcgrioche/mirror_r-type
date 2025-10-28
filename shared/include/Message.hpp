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
#include <iostream>
#include <string>
#include <vector>

enum class MessageType : uint8_t {
    // Client to Server Messages
    CONNECT = 1,
    INPUT = 2,
    PING = 3,
    DISCONNECT = 4,
    CREATE_LOBBY = 5,
    JOIN_LOBBY = 6,
    START_GAME = 7,
    LOBBY_STATE = 8,
    SET_USERNAME = 9,
    KICK_PLAYER = 10,
    AUTH_REQUEST = 11,

    // Server to Client Messages
    CONNECT_ACK = 101,
    GAME_STATE = 102,
    PONG = 103,
    LOBBY_INFO = 104,
    SPAWN_ENTITY = 105,
    DESPAWN_ENTITY = 106,
    ROLLBACK = 107,
    GAME_END_WIN = 108,
    GAME_END_LOSE = 109,
    USERNAME_ACK = 110,
    KICK_ACK = 111,
    KICK_NOTICE = 112,
    AUTH_RESPONSE = 113,
};

class Message {
public:
    uint16_t sequence_number;
    uint32_t player_id;
    uint8_t version;

    /**
     * @brief Constructs a Message with the specified type and optional metadata
     * @param type The message type identifier
     * @param seq Sequence number for message ordering (default: 0)
     * @param pid Player ID associated with the message (default: 0)
     * @param ver Protocol version (default: 1)
     */
    Message(MessageType type, uint16_t seq = 0, uint32_t pid = 0, uint8_t ver = 1);

    /**
     * @brief Constructs a Message with type, payload, and optional metadata
     * @param type The message type identifier
     * @param payload The binary payload data
     * @param seq Sequence number for message ordering (default: 0)
     * @param pid Player ID associated with the message (default: 0)
     * @param ver Protocol version (default: 1)
     */
    Message(MessageType type, const std::vector<uint8_t>& payload, uint16_t seq = 0, uint32_t pid = 0, uint8_t ver = 1);

    /**
     * @brief Copy constructor for Message
     * @param other The Message object to copy
     */
    Message(const Message& other);

    /**
     * @brief Writes an 8-bit unsigned integer to the message payload
     * @param value The value to write
     */
    void write(uint8_t value);

    /**
     * @brief Writes a 16-bit unsigned integer to the message payload
     * @param value The value to write
     */
    void write(uint16_t value);

    /**
     * @brief Writes a 32-bit unsigned integer to the message payload
     * @param value The value to write
     */
    void write(uint32_t value);

    /**
     * @brief Writes a 64-bit unsigned integer to the message payload
     * @param value The value to write
     */
    void write(uint64_t value);

    /**
     * @brief Writes a float value to the message payload
     * @param value The value to write
     */
    void write(float value);

    /**
     * @brief Writes a string to the message payload
     * @param value The string to write
     */
    void write(const std::string& value);

    /**
     * @brief Writes binary data to the message payload
     * @param data The binary data to write
     */
    void write(const std::vector<uint8_t>& data);

    /**
     * @brief Reads an 8-bit unsigned integer from the current read position
     * @return The read value
     */
    uint8_t readU8() const;

    /**
     * @brief Reads a 16-bit unsigned integer from the current read position
     * @return The read value
     */
    uint16_t readU16() const;

    /**
     * @brief Reads a 32-bit unsigned integer from the current read position
     * @return The read value
     */
    uint32_t readU32() const;

    /**
     * @brief Reads a 64-bit unsigned integer from the current read position
     * @return The read value
     */
    uint64_t readU64() const;

    /**
     * @brief Reads a float value from the current read position
     * @return The read value
     */
    float readFloat() const;

    /**
     * @brief Reads a string of specified length from the current read position
     * @param length The length of the string to read
     * @return The read string
     */
    std::string readString(uint8_t length) const;

    /**
     * @brief Reads binary data of specified length from the current read position
     * @param length The number of bytes to read
     * @return The read binary data
     */
    std::vector<uint8_t> readBytes(size_t length) const;

    mutable size_t readPos { 0 };

    /**
     * @brief Serializes the message into a binary format for network transmission
     * @return The serialized message as a byte vector
     */
    std::vector<uint8_t> serialize() const;

    /**
     * @brief Deserializes a binary message back into a Message object
     * @param data The serialized message data
     * @return The deserialized Message object
     */
    static Message deserialize(const std::vector<uint8_t>& data);

    /**
     * @brief Deserializes a message from a buffer at a specific offset
     * @param data The buffer containing message data
     * @param offset The offset to start reading from
     * @param bytesConsumed Output parameter for bytes consumed during deserialization
     * @return The deserialized Message object
     */
    static Message deserializeFromOffset(const std::vector<uint8_t>& data, size_t offset, size_t& bytesConsumed);

    /**
     * @brief Gets the message type
     * @return The message type identifier
     */
    MessageType getType() const { return type; }

    /**
     * @brief Gets the message payload data
     * @return Reference to the payload byte vector
     */
    const std::vector<uint8_t>& getPayload() const { return payload; }

    /**
     * @brief Gets the current read position in the payload
     * @return The current read position index
     */
    size_t getReadPosition() const { return readPos; }

    /**
     * @brief Resets the read position to the beginning of the payload
     */
    void resetReadPosition() { readPos = 0; }

    /**
     * @brief Checks if there are enough bytes remaining to read the specified amount
     * @param bytes The number of bytes to check for
     * @return True if enough bytes are available, false otherwise
     */
    bool canRead(size_t bytes) const { return readPos + bytes <= payload.size(); }

    /**
     * @brief Gets the number of remaining unread bytes in the payload
     * @return The number of remaining bytes
     */
    size_t remainingBytes() const { return payload.size() - readPos; }

    /**
     * @brief Validates that the current read position is within bounds
     * @return True if the read position is valid, false otherwise
     */
    bool isValid() const { return payload.size() >= readPos; }

private:
    MessageType type;
    std::vector<uint8_t> payload;

    static uint16_t byteSwap16(uint16_t value);
    static uint16_t byteSwap16Reverse(uint16_t value);
    static uint32_t byteSwap32(uint32_t value);
    static uint32_t byteSwap32Reverse(uint32_t value);
    static uint64_t byteSwap64(uint64_t value);
    static uint64_t byteSwap64Reverse(uint64_t value);
};

std::ostream& operator<<(std::ostream& cout, MessageType t_type);

#endif // MESSAGE_HPP
