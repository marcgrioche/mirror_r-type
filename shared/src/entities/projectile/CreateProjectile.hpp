/*
** CreateProjectile.hpp for mirror_r-type in /home/jojodelanight/Project/semestre1/mirror_r-type/client/src/projectil
**
** Made by jojo
** Login   <jojo>
**
** Started on  Tue Sep 23 1:12:31 PM 2025 jojo
** Last update Wed Sep 23 1:12:32 PM 2025 jojo
*/

#pragma once
#include "Parent.hpp"
#include "Registry.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/Damage.hpp"
#include "components/Hitbox.hpp"
#include "components/Lifetime.hpp"

namespace factories {
Entity createProjectile(Registry& registry,
    const Position& position,
    const Velocity& velocity,
    const Damage& damage,
    const Hitbox& hitbox,
    const Parent& parent,
    const Lifetime& lifetime);

// Lightweight blueprint entity used only as a template (no Position/Lifetime)
// so it's not processed by gameplay systems (movement/lifetime) and won't expire.
Entity createProjectileTemplate(
    Registry& registry,
    const Velocity& velocity,
    const Damage& damage,
    const Hitbox& hitbox);
}
