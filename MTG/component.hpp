#pragma once
#include "entityx/entityx.h"
#include "common.hpp"
#include "constants.hpp"

#include <functional>

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

// GameState forward declaration
struct GameState;
    
// Metrics used in different components
using Loyalty   = int;
using Mana      = int;
using Power     = int;
using Toughness = int;
    
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
    
} // namespace MTG
