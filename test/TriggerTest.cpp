#include "common.hpp"
#include "gamestate.hpp"
#include "component.hpp"
#include "event.hpp"
#include "factory.hpp"

#include "catch.hpp"

#include <functional>
#include <deque>
#include <iostream>

namespace m   = MTG;
namespace spd = spdlog;
using namespace entityx;

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
        zc->previous = zc->current; // Prevent from triggering again
        DBG("Emitting EnteredBattlefieldEvent({})", ze);
        evt.emit<m::EnteredBattlefieldEvent>({ze});
    }        
};

class GarbageCollectorSystem : public System<GarbageCollectorSystem> {

    void update(EntityManager& mgr, EventManager& evt, TimeDelta dt) {
        DBG("Running garbage collector");
        // Remove orphans
        mgr.each<m::ParentComponent>([](auto e, auto& parent) {
            if(!parent.entity.valid()) {
                DBG("Destroying orphan {}", e);
                e.destroy();
            }
        });
    }
};

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
                
                DBG("Copying effect {}", m::effect(te));
                
                auto effect = mgr.create_from_copy(m::effect(te));
                m::move(effect,m::Zone::STACK);
                m::addParent(effect)->entity = creature;

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
    //m::console->set_level(spd::level::debug);
    m::console->debug("Setting up environment");
    m::GameState game;
    // NOTE: Order here matters
    game.systems.add<TriggerSystem>(game.events, game.entities);
    game.systems.add<ZoneSystem>();
    game.systems.add<GarbageCollectorSystem>();    
    game.systems.configure();
    auto& mgr = game.entities;

    GIVEN("an [ENTERS_BATTLEFIELD] trigger parented to an entity") {
        auto effect  = m::makeEffect(mgr);
        auto parent  = mgr.create();
        auto trigger = m::makeTrigger(
            mgr,
            m::TriggerTiming::ENTERS_BATTLEFIELD,
            m::TriggerLifetime::UNDEFINED,
            effect,
            parent
        );
        
        WHEN("a creature enters the battlefield") {
            auto creature = m::makeCreature(
                mgr, {2}, {2},
                m::CreatureType::HUMAN,
                m::CreatureClass::SOLDIER
            );
            move(creature,m::Zone::BATTLEFIELD);

            // Helpers
            auto isStack = [](auto& zone){return zone.current == m::Zone::STACK;};
            auto checkStackAndParent = [&]{
                Entity effect;
                std::tie(effect, std::ignore) = find<m::ZoneComponent>(mgr,isStack);
                CHECK( effect.valid() );
                CHECK( creature == m::parent(effect) );
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

                THEN("it should be removed when its parent is destroyed") {
                    game.systems.update_all(1.0f);
                    checkStackAndParent();
                    DBG("Destroying effect {}", effect);
                    CHECK( trigger.valid() );
                    
                    parent.destroy();
                    game.systems.update_all(1.0f);
                    CHECK( !trigger.valid() );
                }
            }
        }
    }
}
