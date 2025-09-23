/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** ComponentStorage implementation
*/

#pragma once
#include "ComponentStorage.hpp"

//add or replace component by copy
template <typename Component>
void ComponentStorage<Component>::emplace(Entity e, Component const& comp)
{
    ensure_sparse_size(e.id);
    size_t idx = sparse[e.id];
    if (idx != npos && idx < dense_entities.size() && 
        dense_entities[idx].id == e.id && dense_entities[idx].version == e.version) {
        dense_data[idx] = comp;
        return;
    }
    sparse[e.id] = dense_entities.size();
    dense_entities.push_back(e);
    dense_data.push_back(comp);
}

//add or replace component by move
template <typename Component>
void ComponentStorage<Component>::emplace(Entity e, Component&& comp)
{
    ensure_sparse_size(e.id);
    size_t idx = sparse[e.id];
    if (idx != npos && idx < dense_entities.size() && 
        dense_entities[idx].id == e.id && dense_entities[idx].version == e.version) {
        dense_data[idx] = std::move(comp);
        return;
    }
    sparse[e.id] = dense_entities.size();
    dense_entities.push_back(e);
    dense_data.push_back(std::move(comp));
}

//check if entity has this component
template <typename Component>
bool ComponentStorage<Component>::has(Entity e) const
{
    if (e.id >= sparse.size()) {
        return false;
    }
    size_t idx = sparse[e.id];
    if (idx == npos || idx >= dense_entities.size()) {
        return false;
    }
    Entity stored = dense_entities[idx];
    return stored.id == e.id && stored.version == e.version;
}

//get component reference (mutable)
template <typename Component>
Component& ComponentStorage<Component>::get(Entity e)
{
    size_t idx = sparse.at(e.id);
    assert(idx != npos && idx < dense_entities.size());
    return dense_data[idx];
}

//get component reference (const)
template <typename Component>
const Component& ComponentStorage<Component>::get(Entity e) const
{
    size_t idx = sparse.at(e.id);
    assert(idx != npos && idx < dense_entities.size());
    return dense_data[idx];
}

//remove component from specific entity
template <typename Component>
void ComponentStorage<Component>::remove(Entity e)
{
    if (e.id >= sparse.size()) {
        return;
    }
    size_t idx = sparse[e.id];
    if (idx == npos || idx >= dense_entities.size()) {
        return;
    }
    if (dense_entities[idx].id != e.id || dense_entities[idx].version != e.version) {
        return;
    }
    remove_at_index(idx);
    sparse[e.id] = npos;
}

//remove component by entity id (ignores version)
template <typename Component>
void ComponentStorage<Component>::remove_entity_id(uint32_t entity_id)
{
    if (entity_id >= sparse.size()) {
        return;
    }
    size_t idx = sparse[entity_id];
    if (idx == npos || idx >= dense_entities.size()) {
        return;
    }
    uint32_t moved_entity_id = dense_entities.back().id;
    remove_at_index(idx);
    if (idx < dense_entities.size()) {
        sparse[moved_entity_id] = idx;
    }
    if (entity_id < sparse.size()) sparse[entity_id] = npos;
}

//return number of stored components
template <typename Component>
size_t ComponentStorage<Component>::dense_size() const
{
    return dense_entities.size();
}

//get dense entities array reference
template <typename Component>
const std::vector<Entity>& ComponentStorage<Component>::dense_entities_ref() const
{
    return dense_entities;
}

//get dense components array reference
template <typename Component>
const std::vector<Component>& ComponentStorage<Component>::dense_data_ref() const
{
    return dense_data;
}

//get component by dense array index (mutable)
template <typename Component>
Component& ComponentStorage<Component>::get_by_dense_index(size_t idx)
{
    return dense_data[idx];
}

//get component by dense array index (const)
template <typename Component>
const Component& ComponentStorage<Component>::get_by_dense_index(size_t idx) const
{
    return dense_data[idx];
}

//resize sparse array if needed
template <typename Component>
void ComponentStorage<Component>::ensure_sparse_size(uint32_t entity_id)
{
    if (entity_id >= sparse.size()) {
        sparse.resize(entity_id + 1, npos);
    }
}

//swap-and-pop removal from dense arrays
template <typename Component>
void ComponentStorage<Component>::remove_at_index(size_t idx)
{
    size_t last = dense_entities.size() - 1;
    if (idx != last) {
        dense_entities[idx] = std::move(dense_entities[last]);
        dense_data[idx] = std::move(dense_data[last]);
    }
    dense_entities.pop_back();
    dense_data.pop_back();
}
