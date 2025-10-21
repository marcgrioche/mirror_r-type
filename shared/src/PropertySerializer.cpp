/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Property Serializer Implementation
*/

#include "../include/PropertySerializer.hpp"
#include "../include/EntityData.hpp"
#include <stdexcept>

void PropertySerializer::serializeProperty(Message& msg, const PropertyValue& value, PropertyType type)
{
    switch (type) {
    case PropertyType::INT:
        serializeInt(msg, std::get<int>(value));
        break;
    case PropertyType::UINT32:
        serializeUint32(msg, std::get<uint32_t>(value));
        break;
    case PropertyType::FLOAT:
        serializeFloat(msg, std::get<float>(value));
        break;
    case PropertyType::DOUBLE:
        serializeDouble(msg, std::get<double>(value));
        break;
    case PropertyType::STRING:
        serializeString(msg, std::get<std::string>(value));
        break;
    case PropertyType::VEC2_FLOAT:
        serializeVector2(msg, std::get<Vector2>(value));
        break;
    case PropertyType::VEC4_FLOAT:
        serializeHitbox(msg, std::get<std::array<float, 4>>(value));
        break;
    case PropertyType::BOOL:
        serializeBool(msg, std::get<bool>(value));
        break;
    case PropertyType::ENTITY_ID:
        serializeEntity(msg, std::get<Entity>(value));
        break;
    case PropertyType::POWERUP_TYPE:
        serializePowerUpType(msg, std::get<PowerUpType>(value));
        break;
    default:
        throw std::runtime_error("Unsupported property type for serialization");
    }
}

PropertyValue PropertySerializer::deserializeProperty(Message& msg, PropertyType type)
{
    switch (type) {
    case PropertyType::INT:
        return deserializeInt(msg);
    case PropertyType::UINT32:
        return deserializeUint32(msg);
    case PropertyType::FLOAT:
        return deserializeFloat(msg);
    case PropertyType::DOUBLE:
        return deserializeDouble(msg);
    case PropertyType::STRING:
        return deserializeString(msg);
    case PropertyType::VEC2_FLOAT:
        return deserializeVector2(msg);
    case PropertyType::VEC4_FLOAT:
        return deserializeHitbox(msg);
    case PropertyType::BOOL:
        return deserializeBool(msg);
    case PropertyType::ENTITY_ID:
        return deserializeEntity(msg);
    case PropertyType::POWERUP_TYPE:
        return deserializePowerUpType(msg);
    default:
        throw std::runtime_error("Unsupported property type for deserialization");
    }
}

void PropertySerializer::serializeInt(Message& msg, int value)
{
    msg.write(static_cast<uint32_t>(value));
}

void PropertySerializer::serializeUint32(Message& msg, uint32_t value)
{
    msg.write(value);
}

void PropertySerializer::serializeFloat(Message& msg, float value)
{
    msg.write(value);
}

void PropertySerializer::serializeDouble(Message& msg, double value)
{
    // Convert double to float for network transmission (precision loss acceptable)
    msg.write(static_cast<float>(value));
}

void PropertySerializer::serializeString(Message& msg, const std::string& value)
{
    msg.write(static_cast<uint8_t>(value.length()));
    for (char c : value) {
        msg.write(static_cast<uint8_t>(c));
    }
}

void PropertySerializer::serializeVector2(Message& msg, const Vector2& value)
{
    msg.write(value.x);
    msg.write(value.y);
}

void PropertySerializer::serializeHitbox(Message& msg, const std::array<float, 4>& value)
{
    msg.write(value[0]); // width
    msg.write(value[1]); // height
    msg.write(value[2]); // offset_x
    msg.write(value[3]); // offset_y
}

void PropertySerializer::serializeBool(Message& msg, bool value)
{
    msg.write(static_cast<uint8_t>(value ? 1 : 0));
}

void PropertySerializer::serializeEntity(Message& msg, const Entity& value)
{
    msg.write(static_cast<uint32_t>(value.id));
    msg.write(static_cast<uint32_t>(value.version));
}

void PropertySerializer::serializePowerUpType(Message& msg, PowerUpType value)
{
    msg.write(static_cast<uint8_t>(value));
}

// Deserialization implementations
int PropertySerializer::deserializeInt(Message& msg)
{
    return static_cast<int>(msg.readU32());
}

uint32_t PropertySerializer::deserializeUint32(Message& msg)
{
    return msg.readU32();
}

float PropertySerializer::deserializeFloat(Message& msg)
{
    return msg.readFloat();
}

double PropertySerializer::deserializeDouble(Message& msg)
{
    // Convert float back to double (precision loss occurred during serialization)
    return static_cast<double>(msg.readFloat());
}

std::string PropertySerializer::deserializeString(Message& msg)
{
    uint8_t length = msg.readU8();
    return msg.readString(length);
}

Vector2 PropertySerializer::deserializeVector2(Message& msg)
{
    float x = msg.readFloat();
    float y = msg.readFloat();
    return Vector2 { x, y };
}

std::array<float, 4> PropertySerializer::deserializeHitbox(Message& msg)
{
    float width = msg.readFloat();
    float height = msg.readFloat();
    float offsetX = msg.readFloat();
    float offsetY = msg.readFloat();
    return std::array<float, 4> { width, height, offsetX, offsetY };
}

bool PropertySerializer::deserializeBool(Message& msg)
{
    return msg.readU8() != 0;
}

Entity PropertySerializer::deserializeEntity(Message& msg)
{
    uint32_t id = msg.readU32();
    uint32_t version = msg.readU32();
    return Entity { id, version };
}

PowerUpType PropertySerializer::deserializePowerUpType(Message& msg)
{
    return static_cast<PowerUpType>(msg.readU8());
}
