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

template <typename Component>
class ComponentStorage : public IComponentStorage {
public:
    using component_type = Component;

    ComponentStorage() = default;

    void emplace(Entity e, Component const& comp);
    void emplace(Entity e, Component&& comp);

    bool has(Entity e) const;
    Component& get(Entity e);
    const Component& get(Entity e) const;

    void remove(Entity e);
    void remove_entity_id(uint32_t entity_id) override;

    size_t dense_size() const override;
    const std::vector<Entity>& dense_entities_ref() const;
    const std::vector<Component>& dense_data_ref() const;

    Component& get_by_dense_index(size_t idx);
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
