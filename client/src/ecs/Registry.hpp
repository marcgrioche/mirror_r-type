/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Registry
*/

#pragma once
#include "Components.hpp"
#include "SparseArray.hpp"
#include <any>
#include <optional>
#include <typeindex>
#include <unordered_map>
#include <vector>

class Registry {
public:
    Entity spawn_entity() { return _next_entity_id++; }

    template <typename Component>
    SparseArray<Component>& register_component()
    {
        auto key = std::type_index(typeid(Component));
        if (_components.find(key) == _components.end())
            _components[key] = SparseArray<Component>();
        return std::any_cast<SparseArray<Component>&>(_components[key]);
    }

    template <typename Component>
    SparseArray<Component>& get_components()
    {
        return std::any_cast<SparseArray<Component>&>(_components[std::type_index(typeid(Component))]);
    }

    template <typename Component>
    typename SparseArray<Component>::reference add_component(Entity e, Component&& comp)
    {
        auto& array = register_component<Component>();
        return array.insert_at(e, std::forward<Component>(comp));
    }

    template <typename Component, typename... Args>
    typename SparseArray<Component>::reference emplace_component(Entity e, Args&&... args)
    {
        auto& array = register_component<Component>();
        return array.emplace_at(e, std::forward<Args>(args)...);
    }

    template <typename Component>
    void remove_component(Entity e)
    {
        auto& array = get_components<Component>();
        array.erase(e);
    }

private:
    Entity _next_entity_id = 0;
    std::unordered_map<std::type_index, std::any> _components;
};
