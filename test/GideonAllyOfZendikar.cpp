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
    
// Planswalker
    
struct Loyalty {
    Loyalty(int v = 0) : value{v} {}
    Loyalty(const Loyalty&) = default;
    bool operator==(const Loyalty& rhs) const { return value == rhs.value; }
    bool operator>(int rhs) const { return value > rhs; }
    bool operator<=(int rhs) const { return value <= rhs; }
    Loyalty &operator-=(int rhs) { value -= rhs; return *this; }
    int value;
};
    
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
    
// Trigger
        
enum class TriggerProperties {
    ACTIVE_SELECTION = 0x00000001,
    TARGETED         = 0x00000002,
    CARD_SLOT_0      = 0x00000004,
    CARD_SLOT_1      = 0x00000008,
    CARD_SLOT_2      = 0x00000010,        
};

TriggerProperties operator|(TriggerProperties a, TriggerProperties b) {
    using T = std::underlying_type_t<TriggerProperties>;
    return (TriggerProperties)((T)a | (T)b);
 }

TriggerProperties operator&(TriggerProperties a, TriggerProperties b) {
    using T = std::underlying_type_t<TriggerProperties>;
    return (TriggerProperties)((T)a & (T)b);
 }

bool test(TriggerProperties a) {
    using T = std::underlying_type_t<TriggerProperties>;
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
    
struct TriggerComponent {
    TriggerProperties properties; // Contains information about the trigger
    TriggerCheck      check;      // Checks if the card can be triggered
    TriggerPayment    pay;        // Pays for the trigger
    TriggerAction     action;     // Activate the trigger
};

bool check(GameState& mgr,
           ex::Entity         triggerEntity,
           ex::Entity         targetEntity) {
    auto trigger = triggerEntity.component<TriggerComponent>();
    return parentValid(triggerEntity)
        && trigger->check(mgr,triggerEntity,targetEntity);
}

void pay(GameState& mgr,
         ex::Entity         triggerEntity,
         ex::Entity         targetEntity) {
    auto trigger = triggerEntity.component<TriggerComponent>();
    trigger->pay(mgr,triggerEntity,targetEntity);
}

void action(GameState& mgr,
            ex::Entity         triggerEntity,
            ex::Entity         targetEntity) {
    auto trigger = triggerEntity.component<TriggerComponent>();
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
        
        auto trigger = ability.assign<TriggerComponent>();
        
        trigger->properties = TriggerProperties::ACTIVE_SELECTION
                            | TriggerProperties::TARGETED
                            | TriggerProperties::CARD_SLOT_0;
        
        trigger->check = [](GameState& state, ex::Entity self, auto target) {
            auto gideon  = self.component<ParentComponent>()->entity;
            auto loyalty = gideon.component<PlaneswalkerComponent>()->loyalty;
            return loyalty > 0 && canPlaySorcery(state);
        };

        trigger->pay = [](GameState& state, ex::Entity self, auto target) {
            auto  gideon  = self.component<ParentComponent>()->entity;
            auto& loyalty = gideon.component<PlaneswalkerComponent>()->loyalty;
            loyalty -= 1;
            if(loyalty <= 0) {
                gideon.component<ZoneComponent>()->zone = Zone::GRAVEYARD;
            }
        };

        trigger->action = [](GameState& state, ex::Entity self, auto target) {
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
    ASSERT_EQ(1, count<MTG::TriggerComponent>(mgr,[](auto& component) {
        return test(component.properties & MTG::TriggerProperties::CARD_SLOT_0);
    }));
}

TEST(GideonAllyOfZendikar, test_planeswalker_ability_0_entity) {
    MTG::GameState gs;
    auto& mgr = gs.entities;

    auto gideon = MTG::makeGideonAllyOfZendikar(mgr);
    auto trigger = first<MTG::TriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & MTG::TriggerProperties::CARD_SLOT_0);
    });

    ASSERT_EQ( gideon, trigger.component<MTG::ParentComponent>()->entity );
}

TEST(GideonAllyOfZendikar, test_planeswalker_ability_0_check) {
    MTG::GameState gs;
    auto& mgr = gs.entities;

    auto gideon = MTG::makeGideonAllyOfZendikar(mgr);
    
    auto triggerEntity = first<MTG::TriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & MTG::TriggerProperties::CARD_SLOT_0);
    });

    auto triggerComponent = triggerEntity.component<MTG::TriggerComponent>();

    // Loyalty bad / Phase good
    gs.phase = MTG::Phase::MAIN_1;
    gideon.component<MTG::PlaneswalkerComponent>()->loyalty = {0};
    ASSERT_FALSE( triggerComponent->check(gs, triggerEntity, ex::Entity{}) );

    // Loyalty bad / Phase bad
    gs.phase = MTG::Phase::BEGINNING;
    gideon.component<MTG::PlaneswalkerComponent>()->loyalty = {0};
    ASSERT_FALSE( triggerComponent->check(gs, triggerEntity, ex::Entity{}) );
    
    // Loyalty good / Phase good
    gs.phase = MTG::Phase::MAIN_2;
    gideon.component<MTG::PlaneswalkerComponent>()->loyalty = {1};
    ASSERT_TRUE( triggerComponent->check(gs, triggerEntity, ex::Entity{}) );

    // Loyalty good / Phase bad
    gs.phase = MTG::Phase::BEGINNING;
    gideon.component<MTG::PlaneswalkerComponent>()->loyalty = {1};
    ASSERT_FALSE( triggerComponent->check(gs, triggerEntity, ex::Entity{}) );
}

TEST(GideonAllyOfZendikar, test_planeswalker_ability_0_pay) {
    MTG::GameState gs;
    auto& mgr = gs.entities;

    auto gideon = MTG::makeGideonAllyOfZendikar(mgr);
    
    auto triggerEntity = first<MTG::TriggerComponent>(mgr, [](auto& component) {
        return test(component.properties & MTG::TriggerProperties::CARD_SLOT_0);
    });

    // Survives
    gideon.component<MTG::ZoneComponent>()->zone = MTG::Zone::BATTLEFIELD;
    gideon.component<MTG::PlaneswalkerComponent>()->loyalty = {2};
    MTG::pay(gs, triggerEntity, ex::Entity{});
    ASSERT_EQ( MTG::Zone::BATTLEFIELD,
               gideon.component<MTG::ZoneComponent>()->zone );

    // Moved to graveyard
    gideon.component<MTG::ZoneComponent>()->zone = MTG::Zone::BATTLEFIELD;
    gideon.component<MTG::PlaneswalkerComponent>()->loyalty = {1};
    MTG::pay(gs, triggerEntity, ex::Entity{});
    ASSERT_EQ( MTG::Zone::GRAVEYARD,
               gideon.component<MTG::ZoneComponent>()->zone);
}
