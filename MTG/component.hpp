#pragma once
#include "entityx/entityx.h"
#include "gamestate.hpp"
#include "common.hpp"
#include "constants.hpp"

namespace MTG {
using namespace entityx;

// Component forward declarations
struct PlaneswalkerComponent;
struct ParentComponent;
struct ZoneComponent;
struct CardCostComponent;
struct CreatureComponent;
struct EmblemComponent;
struct MechanicComponent;
struct TriggerComponent;    
struct ManualTriggerComponent; // deprecated
struct EndOfTurnTriggerComponent; // deprecated
struct EnterBattlefieldTriggerComponent; // deprecated
struct BuffComponent;
    
// Metrics used in different components
using Loyalty   = int;
using Mana      = int;
using Power     = int;
using Toughness = int;

// Convenience functions
auto& parent(Entity entity);    
auto  planeswalker(Entity entity);
auto  zone(Entity entity);
template<Zone Z> auto check(Entity entity);
auto  creature(Entity entity);    
auto  mechanic(Entity entity);    
auto  test(ManualTriggerProperties a);
    
////////////////////////////////////////////////////////////////////////////////
// Implementations follow
////////////////////////////////////////////////////////////////////////////////
    
struct PlaneswalkerComponent { Loyalty loyalty; };
struct ParentComponent       { Entity entity; };
struct ZoneComponent         {
    ZoneComponent(const Zone &z = Zone{}) : current(z), previous(z) {}
    Zone current;
    Zone previous;
};
struct CardCostComponent {
    Mana black;
    Mana blue;
    Mana generic;
    Mana green;
    Mana red;
    Mana white;
};
struct CreatureComponent {
    Power power;
    Toughness toughness;
    CreatureType type;
    CreatureClass classs;
};
struct EmblemComponent   { };
struct MechanicComponent { bool summoningSickness; };
    
using TriggerCheck = std::function<
    bool(GameState&, Entity self, Entity target)
>;

using TriggerPayment = std::function< // Need multiple tagets
    void(GameState&, Entity self, Entity target)
>;

using TriggerAction = std::function<
    void(GameState&, Entity self, Entity target)
>;
    
struct ManualTriggerComponent {
    ManualTriggerProperties properties; // Contains information about the trigger
    TriggerCheck            check;      // Checks if the card can be triggered
    TriggerPayment          pay;        // Pays for the trigger
    TriggerAction           action;     // Activate the trigger
};

struct EndOfTurnTriggerComponent {
    TriggerAction action;
};

struct EnterBattlefieldTriggerComponent {
    TriggerAction action;
};

struct TriggerComponent  {
    TriggerTiming   timing;
    TriggerLifetime lifetime;
    Entity          effect;
    Entity          parent;
}; // replaces deprecated triggers
    
// Effect

struct BuffComponent {
    Power      power;
    Toughness  toughness;
    Entity     parent;
};


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
    
} // namespace MTG
