/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** Registry implementation
*/

#include "Registry.hpp"

Entity Registry::create_entity()
{
    if (!free_ids.empty()) {
        uint32_t id = free_ids.back();
        free_ids.pop_back();
        uint32_t ver = ++versions[id];
        return Entity { id, ver };
    } else {
        uint32_t id = static_cast<uint32_t>(versions.size());
        versions.push_back(0);
        return Entity { id, 0 };
    }
}

void Registry::kill_entity(Entity e)
{
    if (e.id >= versions.size()) {
        return;
    }
    versions[e.id]++;

    for (auto& kv : storages) {
        kv.second->remove_entity_id(e.id);
    }
    free_ids.push_back(e.id);
}

uint32_t Registry::get_version(uint32_t id) const
{
    return (id < versions.size()) ? versions[id] : 0;
}
