/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Entity Data Structure
*/

#pragma once

#include "EntityType.hpp"
#include "ecs/Entity.hpp"
#include "ecs/components/Damage.hpp"
#include "ecs/components/Health.hpp"
#include "ecs/components/Hitbox.hpp"
#include "ecs/components/Lifetime.hpp"
#include "ecs/components/Position.hpp"
#include "ecs/components/PowerUp.hpp"
#include "ecs/components/Velocity.hpp"
#include <array>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

/**
 * @brief Value types that can be stored in EntityData
 */
using PropertyValue = std::variant<
    int,
    uint32_t,
    float,
    double,
    std::string,
    Vector2, // Position, Velocity
    std::array<float, 4>, // Hitbox (width, height, offset_x, offset_y)
    bool,
    Entity, // Parent entity
    PowerUpType // Power-up type
    >;

/**
 * @brief Container for entity creation parameters
 * Provides type-safe access to entity properties
 */
class EntityData {
public:
    EntityData(EntityType type)
        : type_(type)
    {
    }

    /**
     * @brief Set a property value
     */
    template <typename T>
    void set(const std::string& key, T value)
    {
        data_[key] = value;
    }

    /**
     * @brief Get a property value with type checking
     */
    template <typename T>
    T get(const std::string& key) const
    {
        auto it = data_.find(key);
        if (it == data_.end()) {
            throw std::runtime_error("Property '" + key + "' not found in EntityData");
        }

        const T* value = std::get_if<T>(&it->second);
        if (!value) {
            throw std::runtime_error("Property '" + key + "' has wrong type");
        }

        return *value;
    }

    /**
     * @brief Get a property value with default
     */
    template <typename T>
    T get(const std::string& key, T defaultValue) const
    {
        auto it = data_.find(key);
        if (it == data_.end()) {
            return defaultValue;
        }

        const T* value = std::get_if<T>(&it->second);
        if (!value) {
            return defaultValue;
        }

        return *value;
    }

    /**
     * @brief Check if a property exists
     */
    bool has(const std::string& key) const
    {
        return data_.find(key) != data_.end();
    }

    /**
     * @brief Get entity type
     */
    EntityType getType() const { return type_; }

    /**
     * @brief Get all property keys
     */
    std::vector<std::string> getKeys() const
    {
        std::vector<std::string> keys;
        keys.reserve(data_.size());
        for (const auto& pair : data_) {
            keys.push_back(pair.first);
        }
        return keys;
    }

    /**
     * @brief Get raw property data (for serialization)
     */
    const std::unordered_map<std::string, PropertyValue>& getData() const
    {
        return data_;
    }

private:
    EntityType type_;
    std::unordered_map<std::string, PropertyValue> data_;
};

/**
 * @brief Builder pattern for EntityData construction
 */
class EntityDataBuilder {
public:
    EntityDataBuilder(EntityType type)
        : data_(type)
    {
    }

    EntityDataBuilder& position(float x, float y)
    {
        data_.set("position", Vector2 { x, y });
        return *this;
    }

    EntityDataBuilder& position(const Position& pos)
    {
        data_.set("position", Vector2 { pos.v.x, pos.v.y });
        return *this;
    }

    EntityDataBuilder& velocity(float x, float y)
    {
        data_.set("velocity", Vector2 { x, y });
        return *this;
    }

    EntityDataBuilder& velocity(const Velocity& vel)
    {
        data_.set("velocity", Vector2 { vel.v.x, vel.v.y });
        return *this;
    }

    EntityDataBuilder& health(int hp)
    {
        data_.set("health", hp);
        return *this;
    }

    EntityDataBuilder& health(const Health& h)
    {
        data_.set("health", h.hp);
        return *this;
    }

    EntityDataBuilder& hitbox(float width, float height, float offsetX = 0.0f, float offsetY = 0.0f)
    {
        data_.set("hitbox", std::array<float, 4> { width, height, offsetX, offsetY });
        return *this;
    }

    EntityDataBuilder& hitbox(const Hitbox& hb)
    {
        data_.set("hitbox", std::array<float, 4> { hb.width, hb.height, hb.offset_x, hb.offset_y });
        return *this;
    }

    EntityDataBuilder& damage(float dmg)
    {
        data_.set("damage", dmg);
        return *this;
    }

    EntityDataBuilder& damage(const Damage& d)
    {
        data_.set("damage", d.value);
        return *this;
    }

    EntityDataBuilder& lifetime(float time)
    {
        data_.set("lifetime", time);
        return *this;
    }

    EntityDataBuilder& lifetime(const Lifetime& lt)
    {
        data_.set("lifetime", lt.value);
        return *this;
    }

    EntityDataBuilder& parent(const Entity& entity)
    {
        data_.set("parent", entity);
        return *this;
    }

    EntityDataBuilder& powerupType(PowerUpType type)
    {
        data_.set("powerup_type", type);
        return *this;
    }

    EntityDataBuilder& powerupEffectDuration(float duration)
    {
        data_.set("effect_duration", duration);
        return *this;
    }

    EntityDataBuilder& username(const std::string& name)
    {
        data_.set("username", name);
        return *this;
    }

    EntityDataBuilder& playerId(uint32_t id)
    {
        data_.set("player_id", id);
        return *this;
    }

    // Generic setter for any property
    template <typename T>
    EntityDataBuilder& set(const std::string& key, T value)
    {
        data_.set(key, value);
        return *this;
    }

    EntityData build()
    {
        return std::move(data_);
    }

private:
    EntityData data_;
};
