/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Registry implementation
*/

#pragma once
#include "Registry.hpp"
#include <stdexcept>

template <typename Component, typename... Args>
void Registry::emplace(Entity e, Args&&... args)
{
    ComponentStorage<Component>* storage = &get_or_create_storage<Component>();
    storage->emplace(e, Component{ std::forward<Args>(args)... });
}

template <typename Component>
void Registry::add(Entity e, Component const& comp)
{
    ComponentStorage<Component>* storage = &get_or_create_storage<Component>();
    storage->emplace(e, comp);
}

template <typename Component>
void Registry::remove(Entity e)
{
    ComponentStorage<Component>* storage = get_storage_if_exists<Component>();
    if (storage) storage->remove(e);
}

template <typename Component>
bool Registry::has(Entity e) const
{
    ComponentStorage<Component>* storage = get_storage_if_exists<Component>();
    return storage ? storage->has(e) : false;
}

template <typename Component>
Component& Registry::get(Entity e)
{
    ComponentStorage<Component>* storage = &get_or_create_storage<Component>();
    return storage->get(e);
}

template <typename Component>
const Component& Registry::get(Entity e) const
{
    ComponentStorage<Component>* storage = get_storage_if_exists<Component>();
    if (!storage) {
        throw std::runtime_error("Component not found");
    }
    return storage->get(e);
}

template <typename Component>
ComponentStorage<Component>& Registry::get_or_create_storage()
{
    std::type_index key(typeid(Component));
    auto it = storages.find(key);
    if (it == storages.end()) {
        auto ptr = std::make_unique<ComponentStorage<Component>>();
        ComponentStorage<Component>* raw = ptr.get();
        storages.emplace(key, std::move(ptr));
        return *raw;
    }
    return *static_cast<ComponentStorage<Component>*>(it->second.get());
}

template <typename Component>
ComponentStorage<Component>* Registry::get_storage_if_exists() const
{
    std::type_index key(typeid(Component));
    auto it = storages.find(key);
    if (it == storages.end()) return nullptr;
    return static_cast<ComponentStorage<Component>*>(it->second.get());
}

template <typename Component>
ComponentStorage<Component>* Registry::get_storage_if_exists()
{
    std::type_index key(typeid(Component));
    auto it = storages.find(key);
    if (it == storages.end()) return nullptr;
    return static_cast<ComponentStorage<Component>*>(it->second.get());
}

template <typename... Components>
Registry::View<Components...>::View(std::tuple<ComponentStorage<Components>*...> st, size_t primary_idx, const std::vector<Entity>* primary_ent)
    : storages(st), primary_index(primary_idx), primary_entities(primary_ent)
{
}

template <typename... Components>
Registry::View<Components...>::iterator::iterator(View const* p, size_t i) : parent(p), idx(i)
{
    advance_to_valid();
}

template <typename... Components>
void Registry::View<Components...>::iterator::advance_to_valid()
{
    if (!parent->primary_entities) {
        idx = 0;
        return;
    }

    while (idx < parent->primary_entities->size()) {
        Entity e = (*parent->primary_entities)[idx];
        if (all_present(e)) break;
        ++idx;
    }
}

template <typename... Components>
bool Registry::View<Components...>::iterator::all_present(Entity const& e) const
{
    iterate_check(e, std::make_index_sequence<sizeof...(Components)>{}, 0);
    return present_cache;
}

template <typename... Components>
template <size_t... Is>
void Registry::View<Components...>::iterator::iterate_check(Entity const& e, std::index_sequence<Is...>, int) const
{
    present_cache = (... && (std::get<Is>(parent->storages) && std::get<Is>(parent->storages)->has(e)));
}

template <typename... Components>
typename Registry::View<Components...>::iterator& Registry::View<Components...>::iterator::operator++()
{
    ++idx;
    advance_to_valid();
    return *this;
}

template <typename... Components>
bool Registry::View<Components...>::iterator::operator!=(iterator const& o) const
{
    return idx != o.idx || parent != o.parent;
}

template <typename... Components>
typename Registry::View<Components...>::value_type Registry::View<Components...>::iterator::operator*() const
{
    if (!parent->primary_entities) {
        throw std::runtime_error("Trying to dereference iterator on empty view");
    }
    Entity e = (*parent->primary_entities)[idx];
    return deref_entity(e, std::make_index_sequence<sizeof...(Components)>{});
}

template <typename... Components>
template <size_t... Is>
typename Registry::View<Components...>::value_type Registry::View<Components...>::iterator::deref_entity(Entity const& e, std::index_sequence<Is...>) const
{
    return std::forward_as_tuple(std::get<Is>(parent->storages)->get(e)...);
}

template <typename... Components>
Entity Registry::View<Components...>::iterator::entity() const
{
    if (!parent->primary_entities) {
        throw std::runtime_error("Trying to get entity from iterator on empty view");
    }
    return (*parent->primary_entities)[idx];
}

template <typename... Components>
typename Registry::View<Components...>::entity_value_type Registry::View<Components...>::iterator::entity_and_components() const
{
    if (!parent->primary_entities) {
        throw std::runtime_error("Trying to dereference iterator on empty view");
    }
    Entity e = (*parent->primary_entities)[idx];
    return std::tuple_cat(std::make_tuple(e), deref_entity(e, std::make_index_sequence<sizeof...(Components)>{}));
}

template <typename... Components>
typename Registry::View<Components...>::iterator Registry::View<Components...>::begin() const
{
    if (!primary_entities) return iterator(this, 0);
    return iterator(this, 0);
}

template <typename... Components>
typename Registry::View<Components...>::iterator Registry::View<Components...>::end() const
{
    if (!primary_entities) return iterator(this, 0);
    return iterator(this, primary_entities->size());
}

// Select the storage with the fewest entities as primary for iteration
template <typename... Components>
Registry::View<Components...> Registry::view()
{
    std::tuple<ComponentStorage<Components>*...> st = std::make_tuple(get_storage_if_exists<Components>()...);

    size_t best_index = SIZE_MAX;
    size_t best_size = SIZE_MAX;
    const std::vector<Entity>* primary_vec = nullptr;

    auto check_one_storage = [&](auto* storage, size_t index) {
        if (storage == nullptr) {
            return;
        }

        size_t current_size = storage->dense_size();
        if (current_size < best_size) {
            best_size = current_size;
            best_index = index;
            primary_vec = &storage->dense_entities_ref();
        }
    };

    // le lambda reçoit un index à la fois, dans le paramètre pack Is...
    // le parameter pack agit comme une boucle, ça va relancer la fonction pour chaque index de la sequence
    auto check_all = [&]<size_t... Is>(std::index_sequence<Is...>) {
        // check le storage à chaque index du tuple st
        (check_one_storage(std::get<Is>(st), Is), ...); //ces "..." sont une fold expression qui répète l'appel pour chaque index
    };

    // sizeof...(Components) = nombre de composants dans la view
    // ex: view<Position, Velocity> → sizeof...(Components) = 2
    // std::make_index_sequence<n>{} génère une séquence d'index de 0 à n-1
    check_all(std::make_index_sequence<sizeof...(Components)>{});

    return View<Components...>(st, best_index, primary_vec);
}
