/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** ComponentStorage
*/

#pragma once
#include "Entity.hpp"
#include <cassert>
#include <limits>
#include <vector>

struct IComponentStorage {
    virtual ~IComponentStorage() = default;
    virtual void remove_entity_id(uint32_t entity_id) = 0;
    virtual size_t dense_size() const = 0;
};

/**
 * @brief Template class for storing ECS components using sparse-set storage
 * @tparam Component The component type to store
 */
template <typename Component>
class ComponentStorage : public IComponentStorage {
public:
    using component_type = Component;

    /**
     * @brief Constructs a ComponentStorage instance
     */
    ComponentStorage() = default;

    /**
     * @brief Adds a component to an entity (copy version)
     * @param e The entity to add the component to
     * @param comp The component to add
     */
    void emplace(Entity e, Component const& comp);

    /**
     * @brief Adds a component to an entity (move version)
     * @param e The entity to add the component to
     * @param comp The component to add
     */
    void emplace(Entity e, Component&& comp);

    /**
     * @brief Checks if an entity has a component
     * @param e The entity to check
     * @return True if the entity has the component, false otherwise
     */
    bool has(Entity e) const;

    /**
     * @brief Gets a mutable reference to an entity's component
     * @param e The entity to get the component from
     * @return Reference to the component
     */
    Component& get(Entity e);

    /**
     * @brief Gets a const reference to an entity's component
     * @param e The entity to get the component from
     * @return Const reference to the component
     */
    const Component& get(Entity e) const;

    /**
     * @brief Removes a component from an entity
     * @param e The entity to remove the component from
     */
    void remove(Entity e);

    /**
     * @brief Removes a component by entity ID
     * @param entity_id The entity ID to remove the component from
     */
    void remove_entity_id(uint32_t entity_id) override;

    /**
     * @brief Gets the number of stored components
     * @return The number of components stored
     */
    size_t dense_size() const override;

    /**
     * @brief Gets a const reference to the dense entities vector
     * @return Const reference to the entities vector
     */
    const std::vector<Entity>& dense_entities_ref() const;

    /**
     * @brief Gets a const reference to the dense data vector
     * @return Const reference to the components vector
     */
    const std::vector<Component>& dense_data_ref() const;

    /**
     * @brief Gets a mutable reference to a component by dense index
     * @param idx The dense index
     * @return Reference to the component
     */
    Component& get_by_dense_index(size_t idx);

    /**
     * @brief Gets a const reference to a component by dense index
     * @param idx The dense index
     * @return Const reference to the component
     */
    const Component& get_by_dense_index(size_t idx) const;

private:
    static constexpr size_t npos = std::numeric_limits<size_t>::max();

    void ensure_sparse_size(uint32_t entity_id);
    void remove_at_index(size_t idx);

    std::vector<Entity> dense_entities;
    std::vector<Component> dense_data;
    std::vector<size_t> sparse;
};

#include "ComponentStorage.tpp"
