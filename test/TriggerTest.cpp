#include "entityx/quick.h"
#include "gamestate.hpp"
#include "component.hpp"
#include "catch.hpp"
#include "event.hpp"

#include <functional>
#include <deque>

namespace m = MTG;
using namespace entityx;

void move(Entity e, m::Zone z) {
    auto component = m::zone(e);
    component->previous = component->current;
    component->current = z;
}

auto makeTrigger(EntityManager&     mgr,
                 m::TriggerTiming   tt,
                 m::TriggerLifetime tl,
                 Entity          effect,
                 Entity          parent = Entity{}) {
    auto entity  =  mgr.create();
    auto trigger = entity.assign<m::TriggerComponent>();
    trigger->timing   = tt;
    trigger->lifetime = tl;
    trigger->effect   = effect;
    if(parent.valid())
        entity.assign<m::ParentComponent>()->entity = parent;
    return trigger;
}

auto makeEffect(EntityManager& mgr) {
    auto entity = mgr.create();
    auto zone = entity.assign<m::ZoneComponent>();
    zone->current =  m::Zone::UNDEFINED;
    zone->previous = m::Zone::UNDEFINED;
    return entity;
}

auto makeCreature(EntityManager& mgr, m::Power power,
                  m::Toughness toughness, m::CreatureType type,
                  m::CreatureClass classs) {
    auto entity = mgr.create();
    auto zone = entity.assign<m::ZoneComponent>();
    zone->current  = m::Zone::UNDEFINED;
    zone->previous = m::Zone::UNDEFINED;
    auto creature = entity.assign<m::CreatureComponent>();
    creature->power     = power;
    creature->toughness = toughness;
    creature->type      = type;
    creature->classs    = classs;
    return entity;
}

class ZoneSystem : public System<ZoneSystem> {
public:
    void update(EntityManager& mgr, EventManager& evt, TimeDelta dt) {
        ComponentHandle<m::ZoneComponent> zone;
        for(Entity ze : mgr.entities_with_components(zone)) {
            enterBattlefield(mgr,evt,dt,ze,zone);
        }
    }
protected:
    void enterBattlefield(EntityManager& mgr,
                          EventManager&  evt,
                          TimeDelta      dt,
                          Entity         ze,
                          ComponentHandle<m::ZoneComponent> zc) {
        static constexpr auto Z = m::Zone::BATTLEFIELD;
        if(Z != zc->current || Z == zc->previous) return;
        evt.emit<m::EnteredBattlefieldEvent>({ze});
    }        
};

/**
 * The trigger system listens to certain events and fires triggers
 * whenever necessary
 */
class TriggerSystem : public System<TriggerSystem>
                    , public Receiver<m::EnteredBattlefieldEvent> {
public:
    TriggerSystem(EventManager& evt, EntityManager& ent) : m_entities{ent} {
        evt.subscribe<m::EnteredBattlefieldEvent>(*this);
    }
    
    void update(EntityManager& mgr, EventManager& evt, TimeDelta dt) {

        // ENTERS_BATTLEFILED triggers should activate on all the
        // spells entering the battlefield. When a trigger is activated
        // it spawns a copy of the spells parent onto the stack. This
        // copy is later removed when the spell resolves
        while(!m_enteredBattlefield.empty()) {
            Entity creature = m_enteredBattlefield.front();
            
            ComponentHandle<m::TriggerComponent> trigger;
            for(Entity te : m_entities.entities_with_components(trigger)) {
                if(m::TriggerTiming::ENTERS_BATTLEFIELD != trigger->timing)
                    continue;
                auto effectCopy = mgr.create_from_copy(m::effect(te));
                effectCopy.assign<m::ParentComponent>()->entity = creature;
                move(effectCopy,m::Zone::STACK);

                if(m::TriggerLifetime::ONCE == trigger->lifetime)
                    te.destroy();
            }
            m_enteredBattlefield.pop_front();
        }
    }

    void receive(const m::EnteredBattlefieldEvent &event) {
        m_enteredBattlefield.push_back(event.entity);
    }

private:
    EntityManager& m_entities;
    #pragma warning "Don't use std::deque"
    std::deque<Entity> m_enteredBattlefield;
};


template<typename TComponent>
auto find(EntityManager& mgr, std::function<bool(const TComponent&)> f) {
    ComponentHandle<TComponent> component;
    for(Entity e : mgr.entities_with_components(component) ){
        if( f(*component) ) return std::make_tuple(e,component);
    }
    // Failure returns invalid
    return std::make_tuple(Entity{},component);
}

SCENARIO("Spells entering the battlefield should activate "
         "[ENTERS_BATTLEFIELD] triggers",
         "[trigger][system]") {
    
    m::GameState game;
    // NOTE: Order here matters
    game.systems.add<TriggerSystem>(game.events, game.entities);
    game.systems.add<ZoneSystem>();
    game.systems.configure();
    auto& mgr = game.entities;

    GIVEN("an [ENTERS_BATTLEFIELD] trigger parented to an entity") {
        constexpr auto timing = m::TriggerTiming::ENTERS_BATTLEFIELD;
        const auto lifetime   = m::TriggerLifetime::UNDEFINED;        
        auto  effect          = makeEffect(mgr);
        auto  trigger         = makeTrigger(mgr, timing, lifetime, effect);
        
        WHEN("a creature enters the battlefield") {
            auto c = makeCreature(mgr, {2}, {2},
                                  m::CreatureType::HUMAN,
                                  m::CreatureClass::SOLDIER);
            move(c,m::Zone::BATTLEFIELD);

            // Helpers
            auto isStack = [](auto& zone){return zone.current == m::Zone::STACK;};
            auto checkStackAndParent = [&]{
                Entity effect;
                std::tie(effect, std::ignore) = find<m::ZoneComponent>(mgr,isStack);
                CHECK( effect.valid() );
                CHECK( c == m::parent(effect) );
            };
            THEN("a copy of the triggers effect should go onto the stack "
                 "with its parent set to the creature that entered the "
                 "battlefield" ) {
                game.systems.update_all(1.0f);
                checkStackAndParent();
            }
            AND_WHEN("the trigger lifetime is [ONCE]") {
                trigger->lifetime = m::TriggerLifetime::ONCE;
                THEN("it should also be destroyed after the effect spawned") {
                    game.systems.update_all(1.0f);
                    checkStackAndParent();
                    CHECK( !trigger.valid() );
                }
            }
            AND_WHEN("the trigger lifetime is [PARENT]") {
                trigger->lifetime = m::TriggerLifetime::PARENT;

                THEN("it should be removed when parent is removed") {
                    game.systems.update_all(1.0f);
                    checkStackAndParent();
                    CHECK( trigger.valid() );
                }
            }
        }
    }
}

// struct ZoneSystemTest : ::testing::Test {
//     virtual void SetUp() {
//         game = new MTG::GameState{};
//         game->systems.add<ZoneSystem>();
//         game->systems.configure();
//     }

//     virtual void TearDown() {
//         delete game;
//     }

//     MTG::GameState* game;
// };

// // Helpers

// auto triggerWithTiming(GameState *game, TriggerTiming t) {
//     auto& mgr       = game->entities;
//     auto  entity    = mgr.create();
//     auto  trigger   = entity.assign<m::TriggerComponent>();
//     trigger->timing = t;
//     return entity;
// }

// auto trigger(Entity e) {
//     return e.component<m::TriggerComponent>();
// }

// void runSystem(GameState *game, TimeDelta dt = 1.0f) {
//     game->systems.update<ZoneSystem>(dt);
// }

// // Mocks

// struct CallbackBase {
//     virtual bool check(GameState& mgr, Entity entity, Entity target) = 0;
//     virtual void pay(GameState& mgr, Entity entity, Entity target) = 0;
//     virtual void action(GameState& mgr, Entity entity, Entity target) = 0;
// };

// struct Callback  {
//     MOCK_METHOD3(check,  bool(GameState&,Entity,Entity));
//     MOCK_METHOD3(pay,    void(GameState&,Entity,Entity));
//     MOCK_METHOD3(action, void(GameState&,Entity,Entity));    
//     void registerCallbacks(GameState *game, Entity e) {
//         trigger(e)->check = [&](GameState& a, Entity b, Entity c) -> bool {
//             return check(a,b,c);
//         };
//         trigger(e)->pay = [&](GameState& a, Entity b, Entity c) {
//             pay(a,b,c);
//         };
//         trigger(e)->action = [&](GameState& a, Entity b, Entity c) {
//             action(a,b,c);
//         };
//     }
// };

// // Tests
// using ::testing::_;

// TEST_F(ZoneSystemTest, trigger_ENTER_BATTLEFIELD) {
//     auto e = triggerWithTiming(game, TriggerTiming::ENTERS_BATTLEFIELD);
//     Callback cb{};    
//     cb.registerCallbacks(game, e);
//     EXPECT_CALL(cb, pay(_,_,_)).Times(1);
//     EXPECT_CALL(cb, action(_,_,_)).Times(1);
//     runSystem(game);
// }
