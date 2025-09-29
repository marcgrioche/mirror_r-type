/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Registry
*/

#pragma once
#include "ComponentStorage.hpp"
#include "Entity.hpp"
#include <algorithm>
#include <memory>
#include <tuple>
#include <typeindex>
#include <unordered_map>
#include <utility>

class Registry {
public:
    Registry() = default;

    Entity create_entity();
    void kill_entity(Entity e);

    template <typename Component, typename... Args>
    void emplace(Entity e, Args&&... args);

    template <typename Component>
    void add(Entity e, Component const& comp);

    template <typename Component>
    void remove(Entity e);

    template <typename Component>
    bool has(Entity e) const;

    template <typename Component>
    Component& get(Entity e);

    template <typename Component>
    const Component& get(Entity e) const;

    template <typename Component>
    ComponentStorage<Component>& get_or_create_storage();

    template <typename Component>
    ComponentStorage<Component>* get_storage_if_exists() const;

    template <typename Component>
    ComponentStorage<Component>* get_storage_if_exists();

    uint32_t get_version(uint32_t id) const;

    // VIEW STRUCT = OPTIMIZED STRUCTURE TO ITERATE OVER ENTITIES HAVING ALL SPECIFIED COMPONENTS
    template <typename... Components>
    struct View {
        std::tuple<ComponentStorage<Components>*...> storages;
        size_t primary_index;
        const std::vector<Entity>* primary_entities = nullptr;
        using value_type = std::tuple<Components&...>;
        using entity_value_type = std::tuple<Entity, Components&...>;

        View(std::tuple<ComponentStorage<Components>*...> st, size_t primary_idx, const std::vector<Entity>* primary_ent);

        // VIEW ITERATOR
        struct iterator {
            View const* parent;
            size_t idx;
            mutable bool present_cache = false;

            iterator(View const* p, size_t i);
            void advance_to_valid();
            bool all_present(Entity const& e) const;

            template <size_t... Is>
            void iterate_check(Entity const& e, std::index_sequence<Is...>, int) const;

            iterator& operator++();
            bool operator!=(iterator const& o) const;
            value_type operator*() const;
            Entity entity() const; // Nouvelle méthode pour récupérer l'entité
            entity_value_type entity_and_components() const; // Méthode pour récupérer entité + composants

            template <size_t... Is>
            value_type deref_entity(Entity const& e, std::index_sequence<Is...>) const;
        };

        iterator begin() const;
        iterator end() const;
    };

    template <typename... Components>
    View<Components...> view();

private:
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> storages;
    std::vector<uint32_t> versions;
    std::vector<uint32_t> free_ids;
};

#include "Registry.tpp"
