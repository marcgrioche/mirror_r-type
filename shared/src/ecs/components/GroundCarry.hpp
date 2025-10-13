#pragma once

/**
 * @brief Component that handles platform carrying mechanics for entities
 * Manages the velocity imparted to entities standing on moving platforms
 */
struct GroundCarry {
    float vx = 0.0f; /**< Horizontal velocity imposed by the platform under the entity */
    float vy = 0.0f; /**< Vertical velocity (for future vertical platform support) */
    bool onPlatform = false; /**< True only on the tick when entity is standing on a platform */
};
