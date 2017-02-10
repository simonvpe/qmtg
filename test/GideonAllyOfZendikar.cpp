#include "gtest/gtest.h"
#include "entityx/quick.h"
#include "cards/cards.hpp"

namespace ex = entityx;

#include <functional>
namespace MTG {

// Game state
enum class Phase {
    BEGINNING,
    MAIN_1,
    COMBAT,
    MAIN_2,
    ENDING
};
    
struct GameState : public ex::EntityX {
    Phase phase;
};

auto canPlaySorcery(GameState &state) {
    return state.phase == Phase::MAIN_1 || state.phase == Phase::MAIN_2;
}

// Util
template<typename Super>
struct Metric {
    Metric(int v = 0) : value{v} {}
    Metric(const Metric&) = default;
    bool    operator==(const Metric& rhs) const { return value == rhs.value; }
    bool    operator> (int rhs)           const { return value > rhs; }
    bool    operator<=(int rhs)           const { return value <= rhs; }
    Metric &operator-=(int rhs)                 { value -= rhs; return *this; }
    int value;    
};
    
// Planswalker
    
struct Loyalty : public Metric<Loyalty> {};
    
struct PlaneswalkerComponent {
    Loyalty loyalty;
};

// Child    
    
struct ParentComponent {
    ex::Entity entity;
};

auto parentValid(ex::Entity entity) {
    return entity.component<ParentComponent>()->entity.valid();
}
    
// Zone
enum class Zone {
    UNDEFINED,
    HAND,
    BATTLEFIELD,
    LIBRARY,
    GRAVEYARD    
};

struct ZoneComponent {
    Zone zone;
};

// Creature
enum class CreatureType {
    HUMAN
};

enum class CreatureClass {
    SOLDIER
};

struct Power : Metric<Power> {};
    
struct Toughness : Metric<Toughness> {};
    
struct CreatureComponent {
    Power         power;
    Toughness     toughness;
    CreatureType  type;
    CreatureClass classs;
};
    
// Trigger
        
enum class ManualTriggerProperties {
    CARD_SLOT_0 = (1 << 0),
    CARD_SLOT_1 = (1 << 1),
    CARD_SLOT_2 = (1 << 2)
};

ManualTriggerProperties operator|(ManualTriggerProperties a, ManualTriggerProperties b) {
    using T = std::underlying_type_t<ManualTriggerProperties>;
    return (ManualTriggerProperties)((T)a | (T)b);
 }

ManualTriggerProperties operator&(ManualTriggerProperties a, ManualTriggerProperties b) {
    using T = std::underlying_type_t<ManualTriggerProperties>;
    return (ManualTriggerProperties)((T)a & (T)b);
 }

bool test(ManualTriggerProperties a) {
    using T = std::underlying_type_t<ManualTriggerProperties>;
    return (T)a != 0;
}
    
using TriggerCheck = std::function<
    bool(GameState&, ex::Entity self, ex::Entity target)
>;

using TriggerPayment = std::function< // Need multiple tagets
    void(GameState&, ex::Entity self, ex::Entity target)
>;

using TriggerAction = std::function<
    void(GameState&, ex::Entity self, ex::Entity target)
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

template<typename TTrigger>
bool check(GameState& mgr,
           ex::Entity         triggerEntity,
           ex::Entity         targetEntity) {
    auto trigger = triggerEntity.component<TTrigger>();
    return parentValid(triggerEntity)
        && trigger->check(mgr,triggerEntity,targetEntity);
}

template<typename TTrigger>    
void pay(GameState& mgr,
         ex::Entity         triggerEntity,
         ex::Entity         targetEntity) {
    auto trigger = triggerEntity.component<TTrigger>();
    trigger->pay(mgr,triggerEntity,targetEntity);
}

template<typename TTrigger>    
void action(GameState& mgr,
            ex::Entity         triggerEntity,
            ex::Entity         targetEntity) {
    auto trigger = triggerEntity.component<TTrigger>();
    trigger->action(mgr,triggerEntity,targetEntity);
}
    
auto makeGideonAllyOfZendikar(ex::EntityManager& mgr) {
    auto gideon = mgr.create();

    auto zone = gideon.assign<ZoneComponent>();
    zone->zone = Zone::UNDEFINED;
    
    auto planeswalker = gideon.assign<PlaneswalkerComponent>();
    planeswalker->loyalty = { 4 };

    // +1
    // ----------------------------------------------------------------------
    // Until end of turn, Gideon, Ally of Zendikar becomes a
    // 5/5 Human Soldier Ally creature with indestructible that's still a
    // planeswalker. Prevent all damage that would be dealt to him this turn.
    // ----------------------------------------------------------------------
    {
        auto ability = mgr.create();
        ability.assign<ParentComponent>()->entity = gideon;
        
        auto trigger = ability.assign<ManualTriggerComponent>();
        
        trigger->properties = ManualTriggerProperties::CARD_SLOT_0;
        
        trigger->check = [](GameState& state, ex::Entity self, auto target) {
            // Enough loyalty points and if sorcery play is allowed
            auto gideon  = self.component<ParentComponent>()->entity;
            auto loyalty = gideon.component<PlaneswalkerComponent>()->loyalty;
            return loyalty > 0 && canPlaySorcery(state);
        };

        trigger->pay = [](GameState& state, ex::Entity self, auto target) {
            // Pay one loyalty, if loyalty reaches 0 move to graveyard
            auto  gideon  = self.component<ParentComponent>()->entity;
            auto& loyalty = gideon.component<PlaneswalkerComponent>()->loyalty;
            loyalty -= 1;
            if(loyalty <= 0) {
                gideon.component<ZoneComponent>()->zone = Zone::GRAVEYARD;
            }
        };

        trigger->action = [](GameState& state, ex::Entity self, auto target) {
            auto gideon = self.component<ParentComponent>()->entity;

            // Change into a creature component
            auto creature = gideon.assign<CreatureComponent>();
            creature->power     = {5};
            creature->toughness = {5};
            creature->type      = CreatureType::HUMAN;
            creature->classs    = CreatureClass::SOLDIER;

            // Remove component when turn ends
            auto endEntity = state.entities.create();
            auto endTrigger = endEntity.assign<EndOfTurnTriggerComponent>();
            endEntity.assign<ParentComponent>()->entity = gideon;
            endTrigger->action = [](GameState& state, ex::Entity self, auto) {
                auto gideon = self.component<ParentComponent>()->entity;
                gideon.remove<CreatureComponent>();
                self.destroy();
            };
        };
        
    }

    return gideon;
}
} // namespace MTG

// Helpers

template<typename TComponent, typename TFunction>
auto count(ex::EntityManager& mgr, TFunction&& f) {
    auto count = 0;
    mgr.each<TComponent>([&](auto e, auto& component) {
        count += f(component) ? 1 : 0;
    });
    return count;
}

template<typename TComponent, typename TFunction>
auto first(ex::EntityManager& mgr, TFunction&& f) {
    ex::Entity result;
    mgr.each<TComponent>([&](auto e, auto& component) {
        result = f(component) && (result == ex::Entity{}) ? e : result;
    });
    return result;
}

// Tests

TEST(GideonAllyOfZendikar, test_planeswalker_loyalty) {
    MTG::GameState gs;
    auto& mgr = gs.entities;
    
    auto e = MTG::makeGideonAllyOfZendikar(mgr);
    auto planeswalker = e.component<MTG::PlaneswalkerComponent>();
    
    ASSERT_EQ( MTG::Loyalty{4}, planeswalker->loyalty );
}

TEST(GideonAllyOfZendikar, test_planeswalker_ability_0_created) {
    MTG::GameState gs;
    auto& mgr = gs.entities;

    auto e = MTG::makeGideonAllyOfZendikar(mgr);
    ASSERT_EQ(1, count<MTG::ManualTriggerComponent>(mgr,[](auto& component) {
        return test(component.properties & MTG::ManualTriggerProperties::CARD_SLOT_0);
    }));
}

TEST(GideonAllyOfZendikar, test_planeswalker_ability_0_entity) {
    MTG::GameState gs;
    auto& mgr = gs.entities;

    auto gideon = MTG::makeGideonAllyOfZendikar(mgr);
    auto trigger = first<MTG::ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & MTG::ManualTriggerProperties::CARD_SLOT_0);
    });

    ASSERT_EQ( gideon, trigger.component<MTG::ParentComponent>()->entity );
}

TEST(GideonAllyOfZendikar, test_planeswalker_ability_0_check) {
    using MTG::ManualTriggerComponent;
    
    MTG::GameState gs;
    auto& mgr = gs.entities;

    auto gideon = MTG::makeGideonAllyOfZendikar(mgr);
    
    auto triggerEntity = first<MTG::ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & MTG::ManualTriggerProperties::CARD_SLOT_0);
    });

    auto triggerComponent = triggerEntity.component<MTG::ManualTriggerComponent>();

    // Loyalty bad / Phase good
    gs.phase = MTG::Phase::MAIN_1;
    gideon.component<MTG::PlaneswalkerComponent>()->loyalty = {0};
    ASSERT_FALSE( MTG::check<ManualTriggerComponent>(
        gs, triggerEntity, ex::Entity{}) );

    // Loyalty bad / Phase bad
    gs.phase = MTG::Phase::BEGINNING;
    gideon.component<MTG::PlaneswalkerComponent>()->loyalty = {0};
    ASSERT_FALSE( MTG::check<ManualTriggerComponent>(
        gs, triggerEntity, ex::Entity{}) );
    
    // Loyalty good / Phase good
    gs.phase = MTG::Phase::MAIN_2;
    gideon.component<MTG::PlaneswalkerComponent>()->loyalty = {1};
    ASSERT_TRUE( MTG::check<ManualTriggerComponent>(
        gs, triggerEntity, ex::Entity{}) );

    // Loyalty good / Phase bad
    gs.phase = MTG::Phase::BEGINNING;
    gideon.component<MTG::PlaneswalkerComponent>()->loyalty = {1};
    ASSERT_FALSE( MTG::check<ManualTriggerComponent>(
        gs, triggerEntity, ex::Entity{}) );
}

TEST(GideonAllyOfZendikar, test_planeswalker_ability_0_pay) {
    using MTG::GameState;
    using MTG::ManualTriggerComponent;
    using MTG::PlaneswalkerComponent;
    using MTG::ManualTriggerProperties;
    using MTG::Zone;
    using MTG::ZoneComponent;
    
    GameState gs;
    auto& mgr = gs.entities;

    auto gideon = MTG::makeGideonAllyOfZendikar(mgr);
    
    auto triggerEntity = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_0);
    });

    // Survives
    gideon.component<ZoneComponent>()->zone = Zone::BATTLEFIELD;
    gideon.component<PlaneswalkerComponent>()->loyalty = {2};
    MTG::pay<ManualTriggerComponent>(gs, triggerEntity, ex::Entity{});
    ASSERT_EQ( Zone::BATTLEFIELD, gideon.component<ZoneComponent>()->zone );

    // Moved to graveyard
    gideon.component<ZoneComponent>()->zone = Zone::BATTLEFIELD;
    gideon.component<PlaneswalkerComponent>()->loyalty = {1};
    MTG::pay<ManualTriggerComponent>(gs, triggerEntity, ex::Entity{});
    ASSERT_EQ( Zone::GRAVEYARD, gideon.component<ZoneComponent>()->zone);
}

TEST(GideonAllyOfZendikar, test_planeswalker_ability_0_action) {
    using MTG::CreatureComponent;
    using MTG::EndOfTurnTriggerComponent;
    using MTG::GameState;
    using MTG::ManualTriggerComponent;
    using MTG::PlaneswalkerComponent;
    using MTG::ManualTriggerProperties;
    using MTG::Zone;
    using MTG::ZoneComponent;
    
    GameState gs;
    auto& mgr = gs.entities;

    auto gideon = MTG::makeGideonAllyOfZendikar(mgr);
    
    auto triggerEntity = first<ManualTriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & ManualTriggerProperties::CARD_SLOT_0);
    });

    EXPECT_FALSE( gideon.component<CreatureComponent>().valid() );
    MTG::action<ManualTriggerComponent>(gs, triggerEntity, ex::Entity{});
    auto creature = gideon.component<CreatureComponent>();
    ASSERT_TRUE( creature.valid() );
    EXPECT_EQ( MTG::Power{5}, creature->power );
    EXPECT_EQ( MTG::Toughness{5}, creature->toughness );
    EXPECT_EQ( MTG::CreatureType::HUMAN, creature->type );
    EXPECT_EQ( MTG::CreatureClass::SOLDIER, creature->classs );

    // Trigger end of turn and make sure Gideons creature is removed
    auto endTrigger = first<EndOfTurnTriggerComponent>(mgr, [](auto& component) {
        return true;
    });
    ASSERT_TRUE( endTrigger.valid() );
    MTG::action<EndOfTurnTriggerComponent>(gs, endTrigger, ex::Entity{} );
    EXPECT_FALSE( endTrigger.valid() );
    EXPECT_FALSE( gideon.component<CreatureComponent>().valid() );
}
