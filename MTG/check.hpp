#pragma once
#include "component.hpp"

namespace MTG {
inline auto& parent(Entity entity) {
    return entity.component<ParentComponent>()->entity;
}

inline auto& effect(Entity entity) {
    return entity.component<TriggerComponent>()->effect;
}
    
inline auto planeswalker(Entity entity) {
    return entity.component<PlaneswalkerComponent>();
}
    
inline auto zone(Entity entity) {
    return entity.component<ZoneComponent>();
}
    
template<Zone Z>
inline auto check(Entity entity) {
    return Z == zone(entity)->current;
}
    
inline auto creature(Entity entity) {
    return entity.component<CreatureComponent>();
}
    
inline auto mechanic(Entity entity) {
    return entity.component<MechanicComponent>();
}
    
inline auto test(ManualTriggerProperties a) {
    using T = std::underlying_type_t<ManualTriggerProperties>;
    return (T)a != 0;
}
    
}
