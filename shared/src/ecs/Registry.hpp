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
    /**
     * @brief Default constructor for Registry
     */
    Registry() = default;

    /**
     * @brief Creates a new entity and returns its handle
     * @return The newly created entity
     */
    Entity create_entity();

    /**
     * @brief Destroys an entity and removes all its components
     * @param e The entity to destroy
     */
    void kill_entity(Entity e);

    /**
     * @brief Constructs and adds a component to an entity in-place
     * @tparam Component The component type to add
     * @tparam Args The argument types for component construction
     * @param e The entity to add the component to
     * @param args The arguments to forward to the component constructor
     */
    template <typename Component, typename... Args>
    void emplace(Entity e, Args&&... args);

    /**
     * @brief Adds a component to an entity by copy
     * @tparam Component The component type to add
     * @param e The entity to add the component to
     * @param comp The component instance to copy
     */
    template <typename Component>
    void add(Entity e, Component const& comp);

    /**
     * @brief Removes a component from an entity
     * @tparam Component The component type to remove
     * @param e The entity to remove the component from
     */
    template <typename Component>
    void remove(Entity e);

    /**
     * @brief Checks if an entity has a specific component
     * @tparam Component The component type to check for
     * @param e The entity to check
     * @return True if the entity has the component, false otherwise
     */
    template <typename Component>
    bool has(Entity e) const;

    /**
     * @brief Gets a mutable reference to a component on an entity
     * @tparam Component The component type to get
     * @param e The entity to get the component from
     * @return Reference to the component
     */
    template <typename Component>
    Component& get(Entity e);

    /**
     * @brief Gets a const reference to a component on an entity
     * @tparam Component The component type to get
     * @param e The entity to get the component from
     * @return Const reference to the component
     */
    template <typename Component>
    const Component& get(Entity e) const;

    /**
     * @brief Gets or creates a component storage for the specified component type
     * @tparam Component The component type
     * @return Reference to the component storage
     */
    template <typename Component>
    ComponentStorage<Component>& get_or_create_storage();

    /**
     * @brief Gets a component storage if it exists (const version)
     * @tparam Component The component type
     * @return Pointer to the component storage, or nullptr if it doesn't exist
     */
    template <typename Component>
    ComponentStorage<Component>* get_storage_if_exists() const;

    /**
     * @brief Gets a component storage if it exists (mutable version)
     * @tparam Component The component type
     * @return Pointer to the component storage, or nullptr if it doesn't exist
     */
    template <typename Component>
    ComponentStorage<Component>* get_storage_if_exists();

    /**
     * @brief Gets the version number for an entity ID
     * @param id The entity ID to get the version for
     * @return The version number
     */
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
