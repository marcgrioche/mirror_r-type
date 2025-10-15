#include "../../include/game_instance/GameInstanceConditions.hpp"
#include "../ecs/components/Health.hpp"
#include "../ecs/components/Tags.hpp"

bool GameInstanceConditions::checkLoseCondition(const Registry& registry,
                                                const std::unordered_map<uint32_t, Entity>& playerEntities)
{
    for (const auto& [playerId, entity] : playerEntities) {
        if (registry.has<Health>(entity) && registry.get<Health>(entity).hp > 0) {
            return false;
        }
    }
    return true;
}

bool GameInstanceConditions::checkWinCondition(const Registry& registry)
{
    // Win if there are no remaining Boss-tagged entities
    const auto* bossStorage = registry.get_storage_if_exists<BossTag>();
    if (bossStorage == nullptr) {
        return true;
    }
    return bossStorage->dense_size() == 0;
}
