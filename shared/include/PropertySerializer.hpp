/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Property Serializer - Type-safe serialization for entity properties
*/

#pragma once

#include "EntityData.hpp"
#include "EntityMetadata.hpp"
#include "Message.hpp"
#include <stdexcept>

/**
 * @brief Handles type-safe serialization and deserialization of entity properties
 * Replaces hardcoded property serialization with automatic type-based handling
 */
class PropertySerializer {
public:
    /**
     * @brief Serialize a property value to a network message
     * @param msg The message to write to
     * @param value The property value to serialize
     * @param type The property type for validation
     * @throws std::runtime_error if type mismatch or unsupported type
     */
    static void serializeProperty(Message& msg, const PropertyValue& value, PropertyType type);

    /**
     * @brief Deserialize a property value from a network message
     * @param msg The message to read from
     * @param type The expected property type
     * @return The deserialized property value
     * @throws std::runtime_error if unsupported type
     */
    static PropertyValue deserializeProperty(Message& msg, PropertyType type);

private:
    // Serialization helpers for each type
    static void serializeInt(Message& msg, int value);
    static void serializeUint32(Message& msg, uint32_t value);
    static void serializeFloat(Message& msg, float value);
    static void serializeDouble(Message& msg, double value);
    static void serializeString(Message& msg, const std::string& value);
    static void serializeVector2(Message& msg, const Vector2& value);
    static void serializeHitbox(Message& msg, const std::array<float, 4>& value);
    static void serializeBool(Message& msg, bool value);
    static void serializeEntity(Message& msg, const Entity& value);
    static void serializePowerUpType(Message& msg, PowerUpType value);

    // Deserialization helpers for each type
    static int deserializeInt(Message& msg);
    static uint32_t deserializeUint32(Message& msg);
    static float deserializeFloat(Message& msg);
    static double deserializeDouble(Message& msg);
    static std::string deserializeString(Message& msg);
    static Vector2 deserializeVector2(Message& msg);
    static std::array<float, 4> deserializeHitbox(Message& msg);
    static bool deserializeBool(Message& msg);
    static Entity deserializeEntity(Message& msg);
    static PowerUpType deserializePowerUpType(Message& msg);
};
